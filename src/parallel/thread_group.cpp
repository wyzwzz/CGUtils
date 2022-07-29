#include <CGUtils/parallel/thread_group.hpp>

namespace wzz::thread{

// --- task_signal_t ---

void task_signal_t::signal_increment()
{
	std::lock_guard<std::mutex> lk(mtx);
	counter++;
	cv.notify_all();
}

void task_signal_t::wait_until_at_least( uint64_t count )
{
	std::unique_lock<std::mutex> lk(mtx);
	cv.wait(lk,[&](){
		return counter >= count;
	});
}

namespace detail{

void task_deps_deleter_t::operator()( task_deps_t * deps)
{
	deps->group->free_task_deps(deps);
}

void task_group_deleter_t::operator()( task_group_t * group)
{
	group->thread_group->free_task_group(group);
}

// --- task_deps_t ---
void task_deps_t::task_completed()
{
	auto old_task_count = count.fetch_sub(1,std::memory_order_acq_rel);
	if(old_task_count == 1)
		notify_dependees();
}

void task_deps_t::dependency_satisfied()
{
	auto old_deps_count = deps_count.fetch_sub(1,std::memory_order_acq_rel);
	assert(old_deps_count > 0);

	if(old_deps_count == 1){
		if(pending_tasks.empty()){
			//notify dependees if pending tasks all completed
			notify_dependees();
		}
		else{
			//move all pending tasks to read tasks
			group->move_to_ready_tasks(pending_tasks);
			pending_deps.clear();
		}
	}
}

void task_deps_t::notify_dependees()
{
	if(signal)
		signal->signal_increment();

	for(auto& dep:pending_deps)
		dep->dependency_satisfied();
	pending_deps.clear();

	{
		std::lock_guard<std::mutex> lk(mtx);
		done = true;
		cv.notify_all();
	}
}


}
// --- task_group_t ---
task_group_t::task_group_t( thread_group_t *group )
:thread_group(group)
{

}

task_group_t::~task_group_t()
{
	if(!flushed)
		flushed;
}

void task_group_t::flush()
{
	if(flushed)
		throw std::logic_error("cannot flush more than once for task group");

	deps->dependency_satisfied();
	flushed = true;
}

void task_group_t::wait()
{
	if(!flushed)
		flush();

	std::unique_lock<std::mutex> lk(deps->mtx);
	deps->cv.wait(lk,[this](){
		return deps->done;
	});
}

void task_group_t::add_flush_dependency()
{
	deps->deps_count.fetch_add(1,std::memory_order_relaxed);
}

void task_group_t::release_flush_dependency()
{
	deps->dependency_satisfied();
}

void task_group_t::enqueue_task( std::function<void()> func )
{
	thread_group->enqueue_task(*this,std::move(func));
}

void task_group_t::set_fence_counter_signal( task_signal_t *signal )
{
	deps->signal = signal;
}

thread_group_t* task_group_t::get_thread_group() const
{
	return thread_group;
}

void task_group_t::set_desc( const char *desc )
{

}

// --- thread_group_t ---

thread_group_t::thread_group_t()
{
	total_tasks.store(0, std::memory_order_relaxed);
	completed_tasks.store(0, std::memory_order_relaxed);
}

thread_group_t::~thread_group_t()
{
	stop();
}

void thread_group_t::start( uint32_t num_threads, const std::function<void()> &on_thread_begin )
{
	assert(num_threads > 0);
	if(active)
		throw std::logic_error("cannot start a thread group which has already started");

	dead = false;
	active = true;

	thread_group.resize(num_threads);

	//start from 1 to n
	uint32_t self_index = 1;
	for(auto& t:thread_group){
		t = std::make_unique<std::thread>([this,on_thread_begin,self_index](){
			// help func
			refresh_global_timeline_trace_file();

			if(on_thread_begin)
				on_thread_begin();

			// thread work loop
			thread_run_looper(self_index);
		});
		++self_index;
	}
}

bool thread_group_t::is_running() const
{
	return active;
}

uint32_t thread_group_t::get_num_threads() const
{
	return static_cast<uint32_t>(thread_group.size());
}

void thread_group_t::stop()
{
	if(!active){
		return;
	}

	//wait all ready tasks enqueue and run
	wait_idle();

	{
		std::lock_guard<std::mutex> lk(mtx);
		dead = true;
		cv.notify_all();
	}

	for(auto& t:thread_group){
		if(t && t->joinable()){
			t->join();
			t.reset();//clear original func
		}
	}

	active = false;
	dead = false;


}

void thread_group_t::enqueue_task( task_group_t &group, std::function<void()> func )
{
	if(group.flushed)
		throw std::logic_error("cannot enqueue work to a flushed task group");

	group.deps->pending_tasks.push_back(task_pool.allocate(group.deps,std::move(func)));
	group.deps->count.fetch_add(1,std::memory_order_relaxed);
}

task_group_handle_t thread_group_t::create_task( std::function<void()> func )
{
	task_group_handle_t  group(task_group_pool.allocate(this));
	group->deps = detail::task_deps_handle_t(task_deps_pool.allocate(this));
	group->deps->pending_tasks.push_back(task_pool.allocate(group->deps,std::move(func)));
	group->deps->count.store(1,std::memory_order_relaxed);
	return group;
}

task_group_handle_t thread_group_t::create_task()
{
	task_group_handle_t  group(task_group_pool.allocate(this));
	group->deps = detail::task_deps_handle_t(task_deps_pool.allocate(this));
	group->deps->count.store(0,std::memory_order_relaxed);
	return group;
}

void thread_group_t::move_to_ready_tasks( const std::vector<detail::task_t *>& task_list )
{
	std::lock_guard<std::mutex> lk(mtx);
	total_tasks.fetch_add(task_list.size(),std::memory_order_relaxed);

	for(auto& t:task_list)
		ready_tasks.push(t);

	if(task_list.size() >= thread_group.size())
		cv.notify_all();
	else{
		auto c = task_list.size();
		for(auto i = 0; i < c; ++i)
			cv.notify_one();
	}
}

void thread_group_t::add_dependency( task_group_t &dependee, task_group_t &dependency )
{
	//dependee has a depend on dependency
	if(dependency.flushed)
		throw std::logic_error("cannot wait for task group which has been flushed");
	if(dependee.flushed)
		throw std::logic_error("cannot add dependency to task group which has been flushed");
	dependency.deps->pending_deps.push_back(dependee.deps);
	dependee.deps->deps_count.fetch_add(1,std::memory_order_relaxed);
}

void thread_group_t::free_task_group( task_group_t *group )
{
	task_group_pool.free(group);
}

void thread_group_t::free_task_deps( detail::task_deps_t *deps )
{
	task_deps_pool.free(deps);
}

void thread_group_t::submit( task_group_handle_t &group )
{
	// flush all tasks in group and reset
	group->flush();
	group.reset();
}

void thread_group_t::wait_idle()
{
	std::unique_lock<std::mutex> lk(wait_mtx);
	wait_cv.wait(lk,[&](){
		return total_tasks.load(std::memory_order_relaxed) == completed_tasks.load(std::memory_order_relaxed);
	});
}

bool thread_group_t::is_idle()
{
	return total_tasks.load(std::memory_order_relaxed) == completed_tasks.load(std::memory_order_relaxed);
}

time::timeline_trace_file_t *thread_group_t::get_timeline_trace_file()
{
	return nullptr;
}

void thread_group_t::refresh_global_timeline_trace_file()
{

}

void thread_group_t::set_async_main_thread_name()
{

}

void thread_group_t::thread_run_looper( uint32_t self_index )
{
	for(;;){

		detail::task_t* task = nullptr;

		//wait until thread dead or ready tasks not empty
		{
			std::unique_lock<std::mutex> lk(mtx);
			cv.wait(lk,[&](){
				return dead || !ready_tasks.empty();
			});

			if(dead && ready_tasks.empty())
				break;

			assert(!ready_tasks.empty());

			task = ready_tasks.front();
			ready_tasks.pop();
		}
		// run the task
		if(task->func){

			task->func();

		}

		task->deps->task_completed();
		task_pool.free(task);

		{
			auto completed = completed_tasks.fetch_add(1,std::memory_order_relaxed) + 1;

			assert(completed <= total_tasks);

			if(completed == total_tasks.load(std::memory_order_relaxed)){
				std::lock_guard<std::mutex> lk(wait_mtx);
				wait_cv.notify_all();
			}
		}

	}
}

void thread_group_t::set_thread_context()
{

}




}
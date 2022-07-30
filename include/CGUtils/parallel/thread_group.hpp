#pragma once
#include <CGUtils/misc.hpp>
#include <CGUtils/time/timeline_trace_file.hpp>
#include <CGUtils/memory/object_pool.hpp>
namespace wzz::thread{

class thread_group_t;

struct task_group_t;

struct task_signal_t{
	std::condition_variable cv;
	std::mutex mtx;
	uint64_t counter = 0;

	void signal_increment();
	void wait_until_at_least(uint64_t count);
};

namespace detail{

struct task_deps_t;
struct task_t;

struct task_deps_deleter_t{
	void operator()(task_deps_t*);
};

struct task_group_deleter_t{
	void operator()(task_group_t*);
};

/**
 * @brief task deps between task groups
 */
struct task_deps_t: misc::intrusive_ptr_enabled_t<task_deps_t,task_deps_deleter_t,misc::multi_thread_counter>{
	explicit task_deps_t(thread_group_t* group)
	:group(group)
	{
		count.store(0,std::memory_order_relaxed);
		// one implicit dependency is the flush() happening.
		deps_count.store(1,std::memory_order_relaxed);
		desc[0] = '\0';
	}
	~task_deps_t() = default;
	void task_completed();
	void dependency_satisfied();
	void notify_dependees();

	thread_group_t* group;

	// 1. wait for all deps completed
	// 2. wait for all tasks completed
	// 3. notify all dependee and done

	//deps count that this deps depend on
	std::atomic_uint deps_count;//note this not equal to pending_deps' size

	//completed tasks count, if all completed should be zero
	//this will be modified when enqueue task / push into pending tasks
	std::atomic_uint count;
	//tasks wait for dependency finished then run
	std::vector<task_t*> pending_tasks;

	//other task deps depend on this task deps
	std::vector<misc::intrusive_ptr_t<task_deps_t>> pending_deps;

	task_signal_t* signal = nullptr;

	std::condition_variable cv;
	std::mutex mtx;
	//if all pending tasks are finished and notified pending deps
	bool done = false;
	char desc[64];

};

using task_deps_handle_t = misc::intrusive_ptr_t<task_deps_t>;

struct task_t{
	task_t(task_deps_handle_t deps,std::function<void()> func)
	:deps(std::move(deps)),func(std::move(func))
	{}
	task_t() = default;

	//deps handle that this task belong
	//used to notify while this tack's func finished
	task_deps_handle_t deps;
	std::function<void()> func;
};

}//end of detail

/**
 * @brief create and use once, can't use any more after flush
 */
struct task_group_t: misc::intrusive_ptr_enabled_t<task_group_t,detail::task_group_deleter_t,misc::multi_thread_counter>{
	//can only create by thread_group_t
	explicit task_group_t(thread_group_t* group);
	~task_group_t();

	//
	void flush();

	// wait for all task completed
	void wait();

	// user control and be care of
	void add_flush_dependency();
	void release_flush_dependency();

	// append a task
	void enqueue_task(std::function<void()> func);
	void set_fence_counter_signal(task_signal_t* signal);
	thread_group_t* get_thread_group() const;

	void set_desc(const char* desc);

	thread_group_t* thread_group;//init by construct
	detail::task_deps_handle_t deps;//allocate by hand
	uint32_t id = 0;
	bool flushed = false;
};

using task_group_handle_t = misc::intrusive_ptr_t<task_group_t>;

class thread_group_t final : misc::no_move_t{
public:
	thread_group_t();
	~thread_group_t();

	void start(uint32_t num_threads,const std::function<void()>& on_thread_begin = nullptr);

	bool is_running() const;

	uint32_t get_num_threads() const;

	void stop();

	//add a task to a created task group
	void enqueue_task(task_group_t& group, std::function<void()> func);
	//create task group with pending task
	task_group_handle_t create_task(std::function<void()> func);
	//create task group without any pending task
	task_group_handle_t create_task();

	//using small vector?
	void move_to_ready_tasks(const std::vector<detail::task_t*>& list);

	void add_dependency(task_group_t& dependee, task_group_t& dependency);

	void free_task_group(task_group_t* group);
	void free_task_deps(detail::task_deps_t* deps);

	//submit will flush and reset the task group, so if a task group submitted if its
	//dependency is not completed, the task group will not execute
	void submit(task_group_handle_t& group);
	void wait_idle();
	bool is_idle();

	time::timeline_trace_file_t* get_timeline_trace_file();
	void refresh_global_timeline_trace_file();

	static void set_async_main_thread_name();
private:
	// actual thread run loop
	void thread_run_looper(uint32_t self_index);
	void set_thread_context();
private:
	//store all tasks including ready_tasks so should explict free a task ptr when finished
	alloc::thread_safe_object_pool_t<detail::task_t> task_pool;
	alloc::thread_safe_object_pool_t<task_group_t> task_group_pool;
	alloc::thread_safe_object_pool_t<detail::task_deps_t> task_deps_pool;

	std::queue<detail::task_t*> ready_tasks;//tasks are ready to run

	std::vector<std::unique_ptr<std::thread>> thread_group;
	std::mutex mtx;
	//cv for tasks not empty or thread dead
	std::condition_variable cv;

	//weather is running, if not can't enqueue task and should start again
	bool active = false;
	//weather the thread should stop if no tasks remain
	bool dead = false;

	//cv for all running tasks completed
	std::condition_variable wait_cv;
	std::mutex wait_mtx;
	std::atomic_uint total_tasks;//total run tasks since create
	std::atomic_uint completed_tasks;//total completed tasks since create

	std::unique_ptr<time::timeline_trace_file_t> timeline_trace_file;
};

}
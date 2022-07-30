#pragma once
#include <thread>
#include <optional>
#include <mutex>
#include "thread_group.hpp"
#include "thread_id.hpp"
namespace wzz::thread{

inline int actual_worker_count(int worker_count) noexcept
{
	if(worker_count <= 0)
		worker_count += static_cast<int>(std::thread::hardware_concurrency());
	return (std::max)(1, worker_count);
}

template<typename Iterable, typename Func>
void parallel_foreach(
  Iterable &&iterable, const Func &func, int worker_count = 0)
{
	std::mutex it_mutex;
	auto it = iterable.begin();
	auto end = iterable.end();
	auto next_item = [&]() -> decltype(std::make_optional(*it))
	{
		std::lock_guard lk(it_mutex);
		if(it == end)
			return std::nullopt;
		return std::make_optional(*it++);
	};

	std::mutex except_mutex;
	std::exception_ptr except_ptr = nullptr;

	worker_count = actual_worker_count(worker_count);

	auto worker_func = [&](int thread_index)
	{
		for(;;)
		{
			auto item = next_item();
			if(!item)
				break;

			try
			{
				func(thread_index, *item);
			}
			catch(...)
			{
				std::lock_guard lk(except_mutex);
				if(!except_ptr)
					except_ptr = std::current_exception();
			}

			std::lock_guard lk(except_mutex);
			if(except_ptr)
				break;
		}
	};

	std::vector<std::thread> workers;
	for(int i = 0; i < worker_count; ++i)
		workers.emplace_back(worker_func, i);

	for(auto &w : workers)
		w.join();

	if(except_ptr)
		std::rethrow_exception(except_ptr);
}

template<typename T, typename Func>
void parallel_forrange(T beg, T end, Func &&func, int worker_count = 0)
{
	std::mutex it_mutex;
	T it = beg;
	auto next_item = [&]() -> std::optional<T>
	{
		std::lock_guard lk(it_mutex);
		if(it == end)
			return std::nullopt;
		return std::make_optional(it++);
	};

	std::mutex except_mutex;
	std::exception_ptr except_ptr = nullptr;

	worker_count = actual_worker_count(worker_count);

	auto worker_func = [&](int thread_index)
	{
		for(;;)
		{
			auto item = next_item();
			if(!item)
				break;

			try
			{
				func(thread_index, *item);
			}
			catch(...)
			{
				std::lock_guard lk(except_mutex);
				if(!except_ptr)
					except_ptr = std::current_exception();
			}

			std::lock_guard lk(except_mutex);
			if(except_ptr)
				break;
		}
	};

	std::vector<std::thread> workers;
	for(int i = 0; i < worker_count; ++i)
		workers.emplace_back(worker_func, i);

	for(auto &w : workers)
		w.join();

	if(except_ptr)
		std::rethrow_exception(except_ptr);
}

/**
 * @param thread_group must start before
 */
template<typename T, typename Func>
void parallel_forrange(T beg, T end, Func &&func, thread_group_t& thread_group,int worker_count = 0)
{
	assert(thread_group.is_running());
	std::mutex it_mutex;
	T it = beg;
	auto next_item = [&]() -> std::optional<T>
	{
		std::lock_guard lk(it_mutex);
		if(it == end)
			return std::nullopt;
		return std::make_optional(it++);
	};

	std::mutex except_mutex;
	std::exception_ptr except_ptr = nullptr;

	int num_threads = thread_group.get_num_threads();
	worker_count = worker_count == 0 ? num_threads : std::min(worker_count, num_threads);

	auto worker_func = [&](int thread_index)
	{
		for(;;)
		{
			auto item = next_item();
			if(!item)
				break;

			try
			{
				func(thread_index, *item);
			}
			catch(...)
			{
				std::lock_guard lk(except_mutex);
				if(!except_ptr)
					except_ptr = std::current_exception();
			}

			std::lock_guard lk(except_mutex);
			if(except_ptr)
				break;
		}
	};

	auto task_group = thread_group.create_task();
	for(int i = 0; i < num_threads; ++i){
		task_group->enqueue_task([&](){
			//note thread group register thread index from 1 to n
			worker_func(get_current_thread_index() - 1);
		});
	}
	thread_group.submit(task_group);
	thread_group.wait_idle();

	if(except_ptr)
		std::rethrow_exception(except_ptr);
}

}
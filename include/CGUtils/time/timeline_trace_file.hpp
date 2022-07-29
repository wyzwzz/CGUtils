#pragma once
#include <string>
#include <thread>
#include <condition_variable>
#include <queue>
#include "../memory/object_pool.hpp"

namespace wzz::time{

class timeline_trace_file_t{
public:
	explicit timeline_trace_file_t(const std::string& path){}
	~timeline_trace_file_t(){}

	static void set_tid(const char* tid);
	static timeline_trace_file_t* get_per_thread();
	static void set_per_thread(timeline_trace_file_t* file);

	struct event_t{
		char desc[256];
		char tid[32];
		uint32_t pid;
		uint64_t start_ns,end_ns;

		void set_desc(const char* desc);
		void set_tid(const char* tid);
	};
	static_assert(alignof(event_t) == 8,"");

	event_t* begin_event(const char* desc, uint32_t pid = 0);
	void end_event(event_t* e);

	event_t* allocate_event();
	void submit_event(event_t* e);
private:
	void looper(const std::string& path);
private:
	std::thread t;
	std::mutex mtx;
	std::condition_variable cv;

	alloc::thread_safe_object_pool_t<event_t> event_pool;
};


}
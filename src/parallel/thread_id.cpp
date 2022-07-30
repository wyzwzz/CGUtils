#include <CGUtils/parallel/thread_id.hpp>
#include <CGUtils/console/logger.hpp>
namespace wzz::thread{

static thread_local uint32_t thread_index = ~0u;

uint32_t get_current_thread_index(){
	if(thread_index == ~0u){
		LOG_ERROR("thread does not exist in thread manager or is not the main thread.");
		return 0;
	}
	return thread_index;
}

void register_thread_index(uint32_t thread_idx){
	thread_index = thread_idx;
}

}
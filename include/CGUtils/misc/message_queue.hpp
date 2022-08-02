#pragma once
#include "ringbuffer.hpp"
#include <memory>
#include <mutex>
namespace wzz::misc{

struct message_queue_payload_deleter_t{
	void operator()(void* ptr);
};

class message_queue_payload_t{
public:
	template <typename T>
	T& as(){
		assert(handle);
		return *static_cast<T*>(handle);
	}

	template <typename T>
	void set_payload_handle(T* t){
		handle = t;
	}

	explicit operator bool() const{
		return bool(payload);
	}

	size_t get_size() const{
		return payload_size;
	}

	void set_size(size_t size){
		assert(size <= payload_capacity);
		payload_size = size;
	}

	void set_payload_data(void* ptr,size_t size){
		payload.reset(ptr);
		payload_capacity = size;
	}

	void* get_payload_data() const{
		return payload.get();
	}

	size_t get_capacity() const{
		return payload_capacity;
	}
private:
	std::unique_ptr<void,message_queue_payload_deleter_t> payload;
	void* handle = nullptr;
	size_t payload_size = 0;
	size_t payload_capacity = 0;
};

class lock_free_message_queue_t{
public:
	lock_free_message_queue_t();

	virtual message_queue_payload_t allocate_write_payload(size_t size) noexcept;
	virtual bool push_written_payload(message_queue_payload_t payload) noexcept;

	virtual size_t available_read_messages() const noexcept;
	virtual message_queue_payload_t read_messages() noexcept;
	virtual void recycle_payload(message_queue_payload_t payload) noexcept;

private:
	static constexpr int WriteRingNum = 8;
	lock_free_ringbuffer_t<message_queue_payload_t> read_ring;
	lock_free_ringbuffer_t<message_queue_payload_t> write_ring[WriteRingNum];
	size_t payload_capacity[WriteRingNum] = {};
};

class message_queue final : private lock_free_message_queue_t{
public:
	message_queue();
	void cork();
	void uncork();

	bool is_uncork() const;

	message_queue_payload_t allocate_write_payload(size_t size) noexcept override;
	bool push_written_payload(message_queue_payload_t payload) noexcept override;

	size_t available_read_messages() const noexcept override;
	message_queue_payload_t read_messages() noexcept override;
	void recycle_payload(message_queue_payload_t payload) noexcept override;

private:
	//todo derived from log interface
	bool lod(const char* tag,const char* fmt,va_list va);
private:
	mutable std::mutex mtx;
	mutable std::condition_variable cv;
	std::atomic_bool corked;
};

}
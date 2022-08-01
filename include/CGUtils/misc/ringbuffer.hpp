#pragma once
#include <vector>
#include <atomic>
#include <cassert>
namespace wzz::misc{


//The C++ standard recommends (but does not require) that lock-free atomic operations are also address-free,
//that is, suitable for communication between processes using shared memory.
// There can only be one concurrent reader, and one concurrent writer.
// Multi writer or reader should use lock be user
template <typename T>
class lock_free_ringbuffer_t{
public:
	lock_free_ringbuffer_t(){
		reset(1);
		assert(read_count.is_lock_free());
		assert(write_count.is_lock_free());
	}

	void reset(size_t count){
		ring.resize(count);
		read_count.store(0);
		write_count.store(0);
	}

	size_t available_read_num() const noexcept{
		return write_count.load(std::memory_order_acquire) - read_count.load(std::memory_order_relaxed);//todo ???
	}


	size_t available_write_num() const noexcept{
		return ring.size() - (write_count.load(std::memory_order_relaxed) - read_count.load(std::memory_order_acquire));
	}

	bool read_and_move(T* values,size_t count) noexcept{
		size_t current_read = read_count.load(std::memory_order_relaxed);
		size_t current_written = write_count.load(std::memory_order_acquire);
		if(count > current_written - current_read)
			return false;

		size_t can_read_first = std::min(ring.size() - read_offset, count);
		size_t can_read_second = count - can_read_first;
		std::move(ring.data() + read_offset, ring.data() + read_offset + can_read_first,values);

		read_offset += can_read_first;
		values += can_read_first;
		if(read_offset >= ring.size())
			read_offset -= ring.size();

		assert(read_offset == 0);

		std::move(ring.data(), ring.data() + can_read_second, values);
		read_offset += can_read_second;

		read_count.store(read_count.load(std::memory_order_relaxed) + count, std::memory_order_release);
		return true;
	}

	bool write_and_move(T* values,size_t count) noexcept{
		size_t current_read = read_count.load(std::memory_order_relaxed);
		size_t current_written = write_count.load(std::memory_order_acquire);
		if(count > ring.size() - (current_written - current_read))
			return false;

		size_t can_write_first = std::min(ring.size() - write_offset,count);
		size_t can_write_second = count - can_write_first;
		std::move(values,values + can_write_first,ring.data() + write_offset);

		write_offset += can_write_first;
		values += can_write_first;
		if(write_offset >= ring.size())
			write_offset -= ring.size();

		assert(write_offset == 0);

		std::move(values,values + can_write_second,ring.data());
		write_offset += can_write_second;

		//need release so prev can't be ordered after the store including load
		write_count.store(write_count.load(std::memory_order_relaxed) + count,std::memory_order_release);
		return true;
	}

	bool read_and_move(T& value) noexcept{
		return read_and_move(&value,1);
	}

	bool write_and_move(T value) noexcept{
		return write_and_move(&value,1);
	}

private:

	std::atomic<size_t> read_count;//total read times
	std::atomic<size_t> write_count;//total write times
	size_t read_offset = 0;//current available read pos begin
	size_t write_offset = 0;//current available write pos begin
	std::vector<T> ring;
};

}
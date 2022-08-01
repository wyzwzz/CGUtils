#include <CGUtils/misc/message_queue.hpp>
#include <CGUtils/memory/alloc.hpp>
namespace wzz::misc{

void message_queue_payload_deleter_t::operator()( void *ptr )
{
	alloc::aligned_free(ptr);
}

lock_free_message_queue_t::lock_free_message_queue_t()
{
	for(int i = 0; i < WriteRingNum; ++i)
		payload_capacity[i] = 256u << i;
	for(int i = 0; i < WriteRingNum; ++i)
		write_ring[i].reset((16u * 1024u) >> i);
	read_ring.reset(32 * 1024);

	for(int i = 0; i < 8; ++i){
		uint32_t count = 512 >> i;
		for(uint32_t j = 0; j < count; ++j){
			message_queue_payload_t payload;
			payload.set_payload_data(alloc::aligned_alloc(payload_capacity[i],64),payload_capacity[i]);
			recycle_payload(std::move(payload));
		}
	}
}

message_queue_payload_t lock_free_message_queue_t::allocate_write_payload( size_t size ) noexcept
{
	message_queue_payload_t payload;
	for(int i = 0; i < WriteRingNum; ++i){
		if(size <= payload_capacity[i]){
			if(!write_ring[i].read_and_move(payload))
				payload.set_payload_data(alloc::aligned_alloc(payload_capacity[i],64),payload_capacity[i]);
			return payload;
		}
	}
	payload.set_payload_data(alloc::aligned_alloc(size,64),size);
	return payload;
}

bool lock_free_message_queue_t::push_written_payload( message_queue_payload_t payload ) noexcept
{
	return read_ring.write_and_move(std::move(payload));
}

size_t lock_free_message_queue_t::available_read_messages() const noexcept
{
	return read_ring.available_read_num();
}

message_queue_payload_t lock_free_message_queue_t::read_messages() noexcept
{
	message_queue_payload_t payload;
	read_ring.read_and_move(payload);
	return payload;
}

void lock_free_message_queue_t::recycle_payload( message_queue_payload_t payload ) noexcept
{
	for(int i = 0; i < WriteRingNum; ++i){
		if(payload.get_capacity() == payload_capacity[i]){
			write_ring[i].write_and_move(std::move(payload));
			return;
		}
	}
}

}
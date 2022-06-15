#pragma once

#include "common.hpp"

namespace wzz::gl{


class buffer_t: public gl_object_base_t{
public:
	explicit buffer_t(bool init_handle = false)

	{}


	buffer_t(buffer_t&& other) noexcept{

	}

	buffer_t& operator=(buffer_t&& other) noexcept{

	}

	~buffer_t(){

	}

	void initialize_handle(){

	}

	void destroy(){

	}

	void reinitialize_buffer_data(const void* data,size_t size,GLenum usage) noexcept{

	}

	void set_buffer_data(const void* data,size_t offset, size_t size) noexcept{

	}

};

template <typename T>
class vertex_buffer_t:public buffer_t{
	size_t vertex_count_;
public:

};

template <typename Block>
class std140_uniform_block_buffer_t:public buffer_t{
public:
	static constexpr int BlockSize = sizeof(Block);

};

template<typename T>
class storage_buffer_t:public buffer_t{

};

}
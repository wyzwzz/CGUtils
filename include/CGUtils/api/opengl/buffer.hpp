#pragma once

#include "common.hpp"

namespace wzz::gl{


class buffer_t: public gl_object_base_t{
public:
	explicit buffer_t(bool init_handle = false)
	:gl_object_base_t(0)
	{
		if(init_handle)
			initialize_handle();
	}


	buffer_t(buffer_t&& other) noexcept
	:gl_object_base_t(other.handle())
	{
		other.handle_ = 0;
	}

	buffer_t& operator=(buffer_t&& other) noexcept{
		destroy();
		this->handle_ = other.handle_;
		other.handle_ = 0;
		return *this;
	}

	~buffer_t(){
		destroy();
	}

	void initialize_handle(){
		assert(!handle_);
		GL_EXPR(glCreateBuffers(1,&handle_));
		if(!handle_)
			throw std::runtime_error("opengl create buffer failed");
	}

	void destroy(){
		if(handle_){
			GL_EXPR(glDeleteBuffers(1,&handle_));
			handle_ = 0;
		}
	}

	void reinitialize_buffer_data(const void* data,size_t size,GLenum usage) noexcept{
		assert(handle_);
		GL_EXPR(glNamedBufferData(handle_,size,data,usage));
	}

	void set_buffer_data(const void* data,size_t offset, size_t size) noexcept{
		assert(handle_);
		GL_EXPR(glNamedBufferSubData(handle_,offset,size,data));
	}

};

/**
 * @tparam T vec3 or struct vertex_t{...};
 */
template <typename T>
class vertex_buffer_t:public buffer_t{
	size_t vertex_count_;
public:
	explicit vertex_buffer_t(bool init_handle = false) noexcept
	:buffer_t(init_handle),vertex_count_(0)
	{

	}

	vertex_buffer_t(const T* data,size_t vertex_count,GLenum usage)
	:buffer_t(true)
	{
			reinitialize_buffer_data(data,vertex_count,usage);
	}

	vertex_buffer_t(vertex_buffer_t&& other) noexcept
	:buffer_t(other.handle()),vertex_count_(other.vertex_count())
	{
		other.handle_ = 0;
		other.vertex_count_ = 0;
	}

	vertex_buffer_t& operator=(vertex_buffer_t&& other) noexcept{
		destroy();
		std::swap(handle_,other.handle_);
		std::swap(vertex_count_,other.vertex_count_);
		return *this;
	}

	~vertex_buffer_t() = default;

	void initialize_handle(){
		buffer_t::initialize_handle();
	}

	void destroy() noexcept{
		buffer_t::destroy();
		vertex_count_ = 0;
	}

	void reinitialize_buffer_data(const T* data,size_t vertex_count,GLenum usage){
		assert(vertex_count);
		buffer_t::reinitialize_buffer_data(data,vertex_count * sizeof(T),usage);
		vertex_count_ = vertex_count;
	}

	void set_buffer_data(const T* data,size_t vertex_offset,size_t vertex_count){
		assert(vertex_count);
		buffer_t::set_buffer_data(data,
								   vertex_offset * sizeof(T),
								   vertex_count * sizeof(T));
	}

	size_t vertex_count() const noexcept{
		return vertex_count_;
	}
};

/**
 * @tparam T uint32_t or uint16_t or other integer type
 */
template <typename T>
class index_buffer_t:public buffer_t{
	size_t index_count_;
public:
	static_assert(std::is_integral_v<T>,"index_buffer_t should used for integer template typename");

	explicit index_buffer_t(bool init_handle = false) noexcept
	  :buffer_t(init_handle),index_count_(0)
	{

	}

	index_buffer_t(const T* data,size_t index_count,GLenum usage)
	:buffer_t(true)
	{
		reinitialize_buffer_data(data,index_count,usage);
	}

	index_buffer_t(index_buffer_t&& other) noexcept
	:buffer_t(other.handle_),index_count_(other.index_count_)
	{
		other.handle_ = 0;
		other.index_count_ = 0;
	}

	index_buffer_t& operator=(index_buffer_t&& other) noexcept{
		destroy();
		std::swap(handle_,other.handle_);
		std::swap(index_count_,other.index_count_);
		return *this;
	}

	~index_buffer_t() = default;

	void initialize_handle(){
		assert(!handle_);
		buffer_t::initialize_handle();
	}

	void destroy() noexcept{
		buffer_t::destroy();
		index_count_ = 0;
	}

	void reinitialize_buffer_data(const T* data,size_t index_count,GLenum usage){
		assert(index_count);
		buffer_t::reinitialize_buffer_data(data,index_count * sizeof(T),usage);
		index_count_ = index_count;
	}

	void set_buffer_data(const T* data,size_t index_offset,size_t index_count){
		assert(index_count);
		buffer_t::set_buffer_data(data,
								   index_offset * sizeof(T),
								   index_count * sizeof(T));
	}

	size_t index_count() const noexcept{
		return index_count_;
	}
};

template <typename Block>
class std140_uniform_block_buffer_t:public buffer_t{
public:
	static constexpr int BlockSize = sizeof(Block);
	static_assert(BlockSize % 16 == 0,"std140 uniform Block's size must be a multiple of 16 bytes");
	explicit std140_uniform_block_buffer_t(bool init_handle = false)
	:buffer_t(init_handle)
	{

	}

	std140_uniform_block_buffer_t(const Block* data,GLenum usage)
	:buffer_t(true)
	{
		reinitialize_buffer_data(data,usage);
	}

	void initialize_handle() noexcept
	{
		buffer_t::initialize_handle();
	}

	void destroy() noexcept
	{
		buffer_t::destroy();
	}

	void reinitialize_buffer_data(const Block *data, GLenum usage) noexcept
	{
		buffer_t::reinitialize_buffer_data(data,BlockSize,usage);
	}

	void set_buffer_data(const void* data,size_t offset,size_t size) noexcept{
		buffer_t::set_buffer_data(data, offset, size);
	}

	void set_buffer_data(const Block* data) noexcept{
		buffer_t::set_buffer_data(data,0,BlockSize);
	}

	template <typename M>
	void set_buffer_data(const M* data,M(Block::* mem)) noexcept{
		buffer_t::set_buffer_data(data, byte_offset(mem), sizeof(M));
	}

	void bind(GLuint binding_point) const noexcept{
		assert(handle_);
		GL_EXPR(glBindBufferBase(GL_UNIFORM_BUFFER,binding_point,handle_));
	}
};

template<typename T>
class storage_buffer_t:public gl_object_base_t{
public:

	explicit storage_buffer_t(bool init_handle = false)
	  :gl_object_base_t(0)
	{
		if(init_handle)
			initialize_handle();
	}
	storage_buffer_t(storage_buffer_t&& other) noexcept
	:gl_object_base_t((other.handle_))
	{
		other.handle_ = 0;
	}

	storage_buffer_t& operator=(storage_buffer_t&& other) noexcept{
		destroy();
		std::swap(handle_,other.handle_);
		return *this;
	}
	~storage_buffer_t(){
		destroy();
	}
	void initialize_handle()
	{
		assert(!handle_);
		GL_EXPR(glCreateBuffers(1,&handle_));
		if(!handle_)
			throw std::runtime_error("opengl create buffer failed");
	}

	void destroy() noexcept
	{
		if(handle_){
			GL_EXPR(glDeleteBuffers(1,&handle_));
			handle_ = 0;
		}
	}

	//just call once after initialize handle
	void initialize_buffer_data(const T *data, size_t size, GLbitfield usage) noexcept
	{
		assert(handle_);
		size_ = size;
		GL_EXPR(glNamedBufferStorage(handle_, size_ * sizeof(T), data, usage));
	}

	void set_buffer_data(const void* data,size_t offset,size_t size_bytes) noexcept{
		assert(handle_);
		GL_EXPR(glNamedBufferSubData(handle_,offset,size_bytes,data));
	}

	void bind(GLuint binding_point) const noexcept{
		assert(handle_);
		GL_EXPR(glBindBufferBase(GL_SHADER_STORAGE_BUFFER,binding_point,handle_));
	}
	void* map(size_t offset,size_t length,GLbitfield access) noexcept{
		assert(handle_);
		GL_EXPR(glMapNamedBufferRange(handle_,offset,length,access));
	}
	void *map(GLenum access){
		assert(handle_);
		GL_EXPR(glMapNamedBuffer(handle_,access));
	}
	void unmap() noexcept{
		assert(handle_);
		GL_EXPR(glUnmapNamedBuffer(handle_));
	}
	size_t size_bytes() const noexcept{
		return size_ * sizeof(T);
	}
	size_t size() const noexcept{
		return size_;
	}

private:
	//element count
	size_t size_ = 0;
};

}
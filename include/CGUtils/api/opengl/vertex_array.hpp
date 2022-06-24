#pragma once

#include "common.hpp"

#include "attrib.hpp"
#include "buffer.hpp"

namespace wzz::gl{

class vertex_array_t: public gl_object_base_t{
public:
	explicit vertex_array_t(bool init_handle = false)
	  : gl_object_base_t(0)
	{
		if(init_handle)
			initialize_handle();
	}

	vertex_array_t(vertex_array_t &&move_from) noexcept
	  : gl_object_base_t(move_from.handle_)
	{
		move_from.handle_ = 0;
	}

	vertex_array_t &operator=(vertex_array_t &&move_from) noexcept
	{
		destroy();
		std::swap(handle_, move_from.handle_);
		return *this;
	}

	~vertex_array_t()
	{
		destroy();
	}

	void initialize_handle()
	{
		assert(!handle_);
		GL_EXPR(glCreateVertexArrays(1, &handle_));
		if(!handle_)
			throw std::runtime_error("failed to create vertex array object");
	}

	void destroy()
	{
		if(handle_)
		{
			GL_EXPR(glDeleteVertexArrays(1, &handle_));
			handle_ = 0;
		}
	}

	template<typename Var>
	void set_attrib_binding_point(
	  attrib_var_t<Var> var, GLuint binding_point) noexcept
	{
		assert(handle_);
		GL_EXPR(glVertexArrayAttribBinding(handle_, var.get_location(), binding_point));
	}


	template<typename Var>
	void set_attrib_format(
	  attrib_var_t<Var> var, GLuint byte_relative_offset,
	  bool normalized) noexcept
	{
		assert(handle_);
		GL_EXPR(glVertexArrayAttribFormat(
		  handle_, var.get_location(),
		  var_to_gl_type<Var>::ncomp, var_to_gl_type<Var>::stype,
		  normalized, byte_relative_offset));
	}

	template<typename Vertex>
	void set_vertex_buffer_binding_point(
	  const vertex_buffer_t<Vertex> &buf, GLuint binding_point,
	  size_t vertex_offset = 0) noexcept
	{
		assert(handle_);
		GL_EXPR(glVertexArrayVertexBuffer(
		  handle_, binding_point, buf.handle(),
		  GLintptr(vertex_offset * sizeof(Vertex)), GLsizei(sizeof(Vertex))));
	}

	//todo
	template<typename T>
	void bind_index_buffer(const index_buffer_t<T>& buf)
	{
		assert(handle_);
		GL_EXPR(glVertexArrayElementBuffer(handle_,buf.handle()));
	}


	template<typename Var>
	void enable_attrib(attrib_var_t<Var> var) noexcept
	{
		assert(handle_);
		GL_EXPR(glEnableVertexArrayAttrib(handle_, var.get_location()));
	}

	template<typename Vertex>
	void bind_vertex_buffer_to_attrib(
	  attrib_var_t<Vertex> var, const vertex_buffer_t<Vertex> &buf,
	  GLuint binding_point, bool normalized = false, size_t vertex_offset = 0) noexcept
	{
		set_attrib_binding_point(var, binding_point);
		set_attrib_format(var, 0, normalized);
		set_vertex_buffer_binding_point(buf, binding_point, vertex_offset);
	}

	template<typename Vertex, typename Var>
	void bind_vertex_buffer_to_attrib(
	  attrib_var_t<Var> var, const vertex_buffer_t<Vertex> &buf, Var (Vertex::*mem),
	  GLuint binding_point, bool normalized = false, size_t vertex_offset = 0) noexcept
	{
		set_attrib_binding_point(var, binding_point);
		set_attrib_format(var, GLuint(byte_offset(mem)), normalized);
		set_vertex_buffer_binding_point(buf, binding_point, vertex_offset);
	}

	void bind() const noexcept
	{
		assert(handle_);
		GL_EXPR(glBindVertexArray(handle_));
	}


	void unbind() const noexcept
	{
		assert(handle_);
		WZZ_WHEN_DEBUG({
			GLint cur;
			glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &cur);
			assert(cur == GLint(handle_));
		});
		GL_EXPR(glBindVertexArray(0));
	}

};

}
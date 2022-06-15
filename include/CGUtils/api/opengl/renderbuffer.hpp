#pragma once

#include "common.hpp"

namespace wzz::gl{

class renderbuffer_t: public gl_object_base_t{
public:
	explicit renderbuffer_t(bool init_handle = false)
	{
		if(init_handle)
			initialize_handle();
	}

	renderbuffer_t(renderbuffer_t &&move_from) noexcept
	  : gl_object_base_t(move_from.handle_)
	{
		move_from.handle_ = 0;
	}

	renderbuffer_t &operator=(renderbuffer_t &&move_from) noexcept
	{
		destroy();
		std::swap(handle_, move_from.handle_);
		return *this;
	}

	~renderbuffer_t()
	{
		destroy();
	}

	void initialize_handle()
	{
		assert(!handle_);
		GL_EXPR(glCreateRenderbuffers(1, &handle_));
		if(!handle_)
			throw std::runtime_error("failed to create renderbuffer object");
	}

	void destroy()
	{
		if(handle_)
		{
			GL_EXPR(glDeleteRenderbuffers(1, &handle_));
			handle_ = 0;
		}
	}

	void set_format(GLenum internal_format, GLsizei width, GLsizei height)
	{
		assert(handle_);
		GL_EXPR(glNamedRenderbufferStorage(handle_, internal_format, width, height));
	}
};


}
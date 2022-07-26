#pragma once

#include "common.hpp"

namespace wzz::gl{
class sampler_t : public gl_object_base_t
{
public:
	explicit sampler_t(bool init_handle = false)
	{
		if(init_handle)
			initialize_handle();
	}

	sampler_t(sampler_t &&move_from) noexcept
	  : gl_object_base_t(move_from.handle_)
	{
		move_from.handle_ = 0;
	}

	sampler_t &operator=(sampler_t &&move_from) noexcept
	{
		destroy();
		std::swap(handle_, move_from.handle_);
		return *this;
	}

	~sampler_t()
	{
		destroy();
	}


	void initialize_handle()
	{
		assert(!handle_);
		glCreateSamplers(1, &handle_);
		if(!handle_)
			throw std::runtime_error("failed to create sampler object");
	}


	void destroy()
	{
		if(handle_)
		{
			GL_EXPR(glDeleteSamplers(1, &handle_));
			handle_ = 0;
		}
	}


	void set_param(GLenum param_name, GLint param_value)
	{
		assert(handle_);
		GL_EXPR(glSamplerParameteri(handle_, param_name, param_value));
	}



	void bind(GLuint texture_unit) const
	{
		assert(handle_);
		GL_EXPR(glBindSampler(texture_unit, handle_));
	}


	void unbind(GLuint texture_uint) const
	{
		assert(handle_);
		GL_EXPR(glBindSampler(texture_uint, 0));
	}
};
}
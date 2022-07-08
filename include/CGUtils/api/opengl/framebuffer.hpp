#pragma once

#include "common.hpp"
#include "texture.hpp"
#include "renderbuffer.hpp"
namespace wzz::gl{

class framebuffer_t: public gl_object_base_t{
public:
	explicit framebuffer_t(bool init_handle = false)
	{
		if(init_handle)
			initialize_handle();
	}
	framebuffer_t(framebuffer_t &&move_from) noexcept
	  : gl_object_base_t(move_from.handle_)
	{
		move_from.handle_ = 0;
	}
	framebuffer_t &operator=(framebuffer_t &&move_from) noexcept
	{
		destroy();
		std::swap(handle_, move_from.handle_);
		return *this;
	}
	~framebuffer_t()
	{
		destroy();
	}
	void initialize_handle()
	{
		assert(!handle_);
		GL_EXPR(glCreateFramebuffers(1, &handle_));
		if(!handle_)
			throw std::runtime_error("failed to create framebuffer object");
	}

	void destroy()
	{
		if(handle_)
		{
			GL_EXPR(glDeleteFramebuffers(1, &handle_));
			handle_ = 0;
		}
	}

	void attach(GLenum attachment, texture2d_t &tex)
	{
		assert(handle_);
		GL_EXPR(glNamedFramebufferTexture(handle_, attachment, tex.handle(), 0));
	}

	void attach(GLenum attachment, renderbuffer_t &buffer)
	{
		assert(handle_);
		GL_EXPR(glNamedFramebufferRenderbuffer(handle_, attachment, GL_RENDERBUFFER, buffer.handle()));
	}
	void bind() noexcept
	{
		assert(is_complete());
		GL_EXPR(glBindFramebuffer(GL_FRAMEBUFFER, handle_));
	}
	void unbind() noexcept
	{
		assert(is_complete());
		WZZ_WHEN_DEBUG({
			GLint cur;
			glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &cur);
			assert(cur == GLint(handle_));
		});
		GL_EXPR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
	bool is_complete() const noexcept
	{
		assert(handle_);
		return glCheckNamedFramebufferStatus(handle_, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	}

	void clear_buffer(GLbitfield buffer) noexcept{
		GL_EXPR(glClear(buffer));
	}

	static void bind_to_default() noexcept{
		GL_EXPR(glBindFramebuffer(GL_FRAMEBUFFER,0));
	}
};

}
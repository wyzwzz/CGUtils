#pragma once

#include "common.hpp"

#include <cassert>
#include <vector>

#include "uniform.hpp"
#include "shader.hpp"
#include "attrib.hpp"
namespace wzz::gl{

class program_t;

namespace detail{
template <GLenum... ShaderTypes>
program_t build(const shader_t<ShaderTypes>&... shaders);
}

class program_t:public gl_object_base_t{
	program_t(GLuint handle) noexcept
	 :gl_object_base_t(handle)
	{
		assert(handle);
	}

	template <GLenum... ShaderTypes>
	friend program_t detail::build(const shader_t<ShaderTypes>&... shaders);
public:

	program_t() = default;

	template <GLenum... ShaderTpyes>
	static program_t build_from(const shader_t<ShaderTpyes>&... shaders);

	program_t(program_t&& other) noexcept
	:gl_object_base_t(other.handle_)
	{
		other.handle_ = 0;
	}

	program_t& operator=(program_t&& other) noexcept{
		destroy();
		handle_ = other.handle_;
		other.handle_ = 0;
		return *this;
	}

	~program_t(){
		destroy();
	}

	void destroy(){
		if(handle_){
			GL_EXPR(glDeleteProgram(handle_));
			handle_ = 0;
		}
	}

	void bind() const noexcept{
		assert(handle_);
		GL_EXPR(glUseProgram(handle_));
	}

	void unbind() const{
		assert(handle_);
		WZZ_WHEN_DEBUG({
		  GLint cur;
		  GL_EXPR(glGetIntegerv(GL_CURRENT_PROGRAM,&cur));
		  assert(cur == handle_);
		});
		GL_EXPR(glUseProgram(0));
	}

	template <typename T>
	void set_uniform_var_unchecked(const char* name,const T& v) const noexcept{
		assert(handle_);
		auto location = glGetUniformLocation(handle_,name);
		if(location < 0){
			LOG_DEBUG("location < 0, {}",name);
		}
		//still set
		detail::set_uniform_value(location,v);
	}

	template <typename T>
	void set_uniform_var(const char* name,const T& v) const{
		assert(handle_ && name);
		auto location = glGetUniformLocation(handle_,name);
		if(location < 0){
			LOG_DEBUG("location < 0, {}",name);
			throw std::runtime_error("set uniform var: location < 0, " + std::string(name));
		}
		detail::set_uniform_value(location,v);
	}

	template <typename T>
	uniform_var_t<T> get_uniform_var(const char* name) const{
		assert(handle_ && name);

		GLuint index;
		GL_EXPR(glGetUniformIndices(handle_,1,&name,&index));
		if(index == GL_INVALID_INDEX){
			throw std::runtime_error("invalid uniform var name: " + std::string(name));
		}
		GLint size;
		GLenum type;
		GL_EXPR(glGetActiveUniform(handle_,index,0,nullptr,&size,&type,nullptr));
		if(type != var_to_gl_type<T>::type){
			throw std::runtime_error("invalid uniform var type of " + std::string(name));
		}

		auto location = glGetUniformLocation(handle_,name);
		assert(location);
		return uniform_var_t<T>(location);
	}

	template <typename Block>
	std140_uniform_block<Block> get_std140_uniform_block(const char* name) const{
		assert(handle_ && name);

		GLuint index = glGetUniformBlockIndex(handle_,name);
		if(index == GL_INVALID_INDEX){
			throw std::runtime_error("invalid uniform var name: " + std::string(name));
		}
		GLint size;
		GL_EXPR(glGetActiveUniformBlockiv(handle_,index,GL_UNIFORM_BLOCK_DATA_SIZE,&size));
		if(sizeof(Block) != size){
			throw std::runtime_error("unmatched std140 uniform block size of " + std::string(name));
		}
		return std140_uniform_block<Block>(handle_,index);
	}

	template <typename T>
	attrib_var_t<T> get_attrib_var(const char* name) const{
		assert(handle_);

		GLint loc = glGetAttribLocation(handle_, name);

		if(loc < 0)
			throw std::runtime_error("invalid attrib variable name: " + std::string(name));

		GLint size; GLenum type;
		glGetActiveAttrib(handle_, loc, 0, nullptr, &size, &type, nullptr);
		if(type != var_to_gl_type<T>::type)
			throw std::runtime_error("unmatched attrib variable type of " + std::string(name));

		return attrib_var_t<T>(loc);

	}
};

namespace detail{

	template<GLenum ShaderType>
	auto add_shader(std::vector<GLuint>& shader_handles,const shader_t<ShaderType>& shader){
		shader_handles.push_back(shader.handle());
	}

	template<GLenum ShaderType,GLenum... ShaderTypes>
	auto add_shader(std::vector<GLuint>& shader_handles,const shader_t<ShaderType>& shader,const shader_t<ShaderTypes>&... others){
		add_shader(shader_handles,shader);
		add_shader(shader_handles,others...);
	}

	template <GLenum... ShaderTypes>
	program_t build(const shader_t<ShaderTypes>&... shaders){
		std::vector<GLuint> shader_handles;
		add_shader(shader_handles,shaders...);
		GLuint handle = glCreateProgram();
		if(!handle){
			throw std::runtime_error("failed to create opengl program");
		}

		misc::scope_bomb_t guard([=](){
			if(handle){
				std::vector<GLuint> attached_shaders(shader_handles.size());
				GLsizei count;
				GL_EXPR(glGetAttachedShaders(handle, GLsizei(attached_shaders.size()),&count, attached_shaders.data()));
				for(GLsizei i = 0; i < count; ++i)
					GL_EXPR(glDetachShader(handle, attached_shaders[i]));
				GL_EXPR(glDeleteProgram(handle));
			}
		});

		for(auto shader_handle:shader_handles){
			GL_EXPR(glAttachShader(handle,shader_handle));
		}
		GL_EXPR(glLinkProgram(handle));
		GLint ret;
		GL_EXPR(glGetProgramiv(handle,GL_LINK_STATUS,&ret));
		if(ret != GL_TRUE){
			GLint log;
			glGetProgramiv(handle,GL_INFO_LOG_LENGTH,&log);
			std::vector<char> log_buf(log + 1);
			glGetProgramInfoLog(handle,log_buf.size() + 1,nullptr,log_buf.data());
			std::string info = "Program link info:\n";
			info.append(log_buf.data(),log_buf.size());
			throw std::runtime_error(info.data());
		}

		for(auto shader_handle:shader_handles){
			GL_EXPR(glDetachShader(handle,shader_handle));
		}

		guard.dismiss();
		return program_t(handle);
}

}

template <GLenum... ShaderTpyes>
program_t program_t::build_from( const shader_t<ShaderTpyes> &...shaders )
{
	return detail::build(shaders...);
}

}
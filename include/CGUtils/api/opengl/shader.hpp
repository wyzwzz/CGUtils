#pragma once

#include "common.hpp"

#include <CGUtils/file.hpp>

namespace wzz::gl{

inline std::string get_shader_type_name(GLenum T){
	switch ( T ) {
	case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
	case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
	case GL_GEOMETRY_SHADER: return "GL_GEOMETRY_SHADER";
	case GL_COMPUTE_SHADER: return "GL_COMPUTE_SHADER";
	default: return "UNKNOWN SHADER TYPE";
	}
}

template<GLenum T>
class shader_t: public gl_object_base_t{
public:
	static constexpr GLenum ShaderType = T;

	shader_t() = default;

	shader_t(shader_t&&) noexcept = default;

	shader_t& operator=(shader_t&& other) noexcept{
		destroy();
		handle_ = other.handle_;
		other.handle_ = 0;
		return *this;
	}

	~shader_t(){
		destroy();
	}

	void destroy(){
		if(handle_){
			glDeleteShader(handle_);
			handle_ = 0;
		}
	}

	static shader_t from_file(const std::string& filename){
		shader_t ret;
		ret.load_from_file(filename);
		return std::move(ret);
	}

	static shader_t from_memory(const std::string_view src){
		shader_t ret;
		ret.load_from_memory(src);
		return std::move(ret);
	}

	void load_from_file(const std::string& filename){
		load_from_memory(file::read_txt_file(filename),filename);
	}

	void load_from_memory(std::string_view src,std::string filename = ""){
		destroy();

		GLuint new_handle = glCreateShader(ShaderType);
		if(!new_handle)
			throw std::runtime_error("failed to create shader object");

		const char* src_data = src.data();
		GLint src_size = static_cast<GLint>(src.length());
		GL_EXPR(glShaderSource(new_handle,1,&src_data,&src_size));

		GLint ret;
		GL_EXPR(glCompileShader(new_handle));
		GL_EXPR(glGetShaderiv(new_handle,GL_COMPILE_STATUS,&ret));
		if(ret != GL_TRUE){
			GLint log;
			GL_EXPR(glGetShaderiv(new_handle,GL_INFO_LOG_LENGTH,&log));
			std::vector<char> log_buf(log + 1);
			GL_EXPR(glGetShaderInfoLog(new_handle,log + 1, nullptr, log_buf.data()));
			GL_EXPR(glDeleteShader(new_handle));
			filename = get_shader_type_name(T) + "Compile Info:\n" +
					   filename + "\n";
			filename.append(log_buf.data(),log_buf.size());
			throw std::runtime_error(filename.data());
		}
		handle_ = new_handle;
	}
};

using vertex_shader_t = shader_t<GL_VERTEX_SHADER>;
using geometry_shader_t = shader_t<GL_GEOMETRY_SHADER>;
using fragment_shader_t = shader_t<GL_FRAGMENT_SHADER>;
using compute_shader_t = shader_t<GL_COMPUTE_SHADER>;

}
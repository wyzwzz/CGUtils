#pragma once

#include "common.hpp"

namespace wzz::gl{

namespace detail{
	inline void set_uniform_value(GLint location, GLfloat x) {GL_EXPR(glUniform1f(location,x));}
	inline void set_uniform_value(GLint location, const vec2& v) {GL_EXPR(glUniform2f(location,v.x,v.y));}
	inline void set_uniform_value(GLint location, const vec3& v) {GL_EXPR(glUniform3f(location,v.x,v.y,v.z));}
	inline void set_uniform_value(GLint location, const vec4& v) {GL_EXPR(glUniform4f(location,v.x,v.y,v.z,v.w));}
	inline void set_uniform_value(GLint location, GLint x) {GL_EXPR(glUniform1i(location,x));}
	inline void set_uniform_value(GLint location, GLuint x) {GL_EXPR(glUniform1ui(location,x));}
	inline void set_uniform_value(GLint location, const vec2i& v) {GL_EXPR(glUniform2i(location,v.x,v.y));}
	inline void set_uniform_value(GLint location, const vec3i& v) {GL_EXPR(glUniform3i(location,v.x,v.y,v.z));}
	inline void set_uniform_value(GLint location, const vec4i& v) {GL_EXPR(glUniform4i(location,v.x,v.y,v.z,v.w));}
	inline void set_uniform_value(GLint location, const mat4& m, bool transpose = false) {GL_EXPR(glUniformMatrix4fv(location,1,transpose,&m[0][0]));}
	inline void set_uniform_value(GLint location, const texture1d_unit_t& unit) {GL_EXPR(glUniform1i(location,unit.unit));}
	inline void set_uniform_value(GLint location, const texture2d_unit_t& unit) {GL_EXPR(glUniform1i(location,unit.unit));}
	inline void set_uniform_value(GLint location, const texture3d_unit_t& unit) {GL_EXPR(glUniform1i(location,unit.unit));}

}

template <typename T>
class uniform_var_t{
	GLint location;
public:
	uniform_var_t(GLint location = 0) noexcept
	:location(location)
	{}

	void set_uniform_value(const T& v) const{
		detail::set_uniform_value(location,v);
	}

	GLint get_location() const noexcept{
		return location;
	}
};

template <typename Block>
class std140_uniform_block{
	GLuint program;
	GLuint idx;
public:
	explicit std140_uniform_block(GLuint program,GLuint index = 0) noexcept
	:program(program),idx(index)
	{
		assert(program);
	}

	GLuint index() const noexcept{
		return idx;
	}
	void bind(GLuint binding_point) const noexcept{
		assert(program);
		GL_EXPR(glUniformBlockBinding(program,idx, binding_point));
	}
};

}
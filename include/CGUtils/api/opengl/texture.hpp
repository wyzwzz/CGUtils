#pragma once

#include "common.hpp"
#include <CGUtils/texture.hpp>
namespace wzz::gl{

namespace detail{
	template <typename DataTexel> struct dt_to_tt;

	template <>
	struct dt_to_tt<GLfloat>{
		static constexpr GLenum format = GL_RED;
		static constexpr GLenum type = GL_FLOAT;
		static constexpr GLint row_align = 4;
	};

	template<>
	struct dt_to_tt<vec3>{
		static constexpr GLenum format = GL_RGB;
		static constexpr GLenum type = GL_FLOAT;
		static constexpr GLint row_align = 4;
	};

	template<>
	struct dt_to_tt<vec4>{
		static constexpr GLenum format = GL_RGBA;
		static constexpr GLenum type = GL_FLOAT;
		static constexpr GLint row_align = 4;
	};

	template<>
	struct dt_to_tt<math::color3f>{
		static constexpr GLenum format = GL_RGB;
		static constexpr GLenum type = GL_FLOAT;
		static constexpr GLint row_align = 4;
	};

	template<>
	struct dt_to_tt<math::color4f>{
		static constexpr GLenum format = GL_RGBA;
		static constexpr GLenum type = GL_FLOAT;
		static constexpr GLint row_align = 4;
	};

	template <>
	struct dt_to_tt<GLubyte>{
		static constexpr GLenum format = GL_RED;
		static constexpr GLenum type = GL_UNSIGNED_BYTE;
		static constexpr GLint row_align = 1;
	};

	template<>
	struct dt_to_tt<vec3b>{
		static constexpr GLenum format = GL_RGB;
		static constexpr GLenum type = GL_UNSIGNED_BYTE;
		static constexpr GLint row_align = 1;
	};

	template<>
	struct dt_to_tt<vec4b>{
		static constexpr GLenum format = GL_RGBA;
		static constexpr GLenum type = GL_UNSIGNED_BYTE;
		static constexpr GLint row_align = 4;
	};

	template<>
	struct dt_to_tt<math::color3b>{
		static constexpr GLenum format = GL_RGB;
		static constexpr GLenum type = GL_UNSIGNED_BYTE;
		static constexpr GLint row_align = 1;
	};

	template<>
	struct dt_to_tt<math::color4b>{
		static constexpr GLenum format = GL_RGBA;
		static constexpr GLenum type = GL_UNSIGNED_BYTE;
		static constexpr GLint row_align = 4;
	};

	template<>
	struct dt_to_tt<GLuint>{
		static constexpr GLenum format = GL_R;
		static constexpr GLenum type = GL_UNSIGNED_INT;
		static constexpr GLint row_align = 4;
	};


}

class texture1d_t: public	gl_object_base_t{
public:
	explicit texture1d_t(bool init_handle = false){
		if(init_handle)
			initialize_handle();
	}

	texture1d_t(texture1d_t&& other) noexcept
	  :gl_object_base_t(other.handle_)
	{
		other.handle_ = 0;
	}

	texture1d_t& operator=(texture1d_t&& other) noexcept{
		destroy();
		std::swap(handle_,other.handle_);
		return *this;
	}

	~texture1d_t(){
		destroy();
	}

	void initialize_handle(){
		assert(!handle_);
		GL_EXPR(glCreateTextures(GL_TEXTURE_1D,1,&handle_));
		if(!handle_)
			throw std::runtime_error("failed to create opengl texture1d object");
	}

	void destroy(){
		if(handle_){
			GL_EXPR(glDeleteTextures(1,&handle_));
			handle_ = 0;
		}
	}

	template <typename DataT>
	void initialize_format_and_data(GLsizei levels, GLenum internal_format,
									 GLsizei width, const DataT *data){
		initialize_texture(levels,internal_format,width);
		set_texture_data(width,data);
	}

	template <typename DataT>
	void initialize_format_and_data(GLsizei levels,GLenum internal_format,
									 const texture::image1d_t<DataT>& data){
		initialize_texture(levels,internal_format,data.width());
		set_texture_data(data.width(),data.get_raw_data());
	}
	template <typename DataT>
	void initialize_format_and_data(GLsizei levels,GLenum internal_format,
									 const math::tensor_t<DataT,1>& data){
		initialize_texture(levels,internal_format,data.shape()[0]);
		set_texture_data(data.shape()[0],data.raw_data());
	}

	void initialize_texture(GLsizei levels,GLenum internal_format,
							 GLsizei width){
		assert(handle_);
		GL_EXPR(glTextureStorage1D(handle_,levels,internal_format,width));
	}

	template <typename DataT>
	void set_texture_data(GLsizei width,const DataT* data){
		assert(handle_);
		GLint old_row_align;
		GL_EXPR(glGetIntegerv(GL_UNPACK_ALIGNMENT,&old_row_align));
		GL_EXPR(glPixelStorei(GL_UNPACK_ALIGNMENT,
								detail::dt_to_tt<DataT>::row_align));
		GL_EXPR(glTextureSubImage1D(
		  handle_,0,0,width,
		  detail::dt_to_tt<DataT>::format,
		  detail::dt_to_tt<DataT>::type,data
		  ));
		GL_EXPR(glPixelStorei(GL_UNPACK_ALIGNMENT,old_row_align));
		GL_EXPR(glGenerateTextureMipmap(handle_));
	}

	void set_texture_param(GLenum param_name,GLint param_value){
		assert(handle_);
		GL_EXPR(glTextureParameteri(handle_,param_name,param_value));
	}

	void bind(GLuint texture_unit) const noexcept{
		assert(handle_);
		GL_EXPR(glBindTextureUnit(texture_unit,handle_));
	}
	void bind_image(int binding_point,int level,GLenum access,GLenum format) const noexcept{
		GL_EXPR(glBindImageTexture(binding_point,handle_,level,GL_FALSE,0,access,format));
	}
	void unbind(GLuint texture_unit) const noexcept{
		assert(handle_);
		GL_EXPR(glBindTextureUnit(texture_unit,0));
	}
};

class texture2d_t: public gl_object_base_t{
public:
	explicit texture2d_t(bool init_handle = false){
		if(init_handle)
			initialize_handle();
	}

	texture2d_t(texture2d_t&& other) noexcept
	:gl_object_base_t(other.handle_)
	{
		other.handle_ = 0;
	}

	texture2d_t& operator=(texture2d_t&& other) noexcept{
		destroy();
		std::swap(handle_,other.handle_);
		return *this;
	}

	~texture2d_t(){
		destroy();
	}

	void initialize_handle(){
		assert(!handle_);
		GL_EXPR(glCreateTextures(GL_TEXTURE_2D,1,&handle_));
		if(!handle_)
			throw std::runtime_error("failed to create opengl texture2d object");
	}

	void destroy(){
		if(handle_){
			GL_EXPR(glDeleteTextures(1,&handle_));
			handle_ = 0;
		}
	}

	template <typename DataT>
	void initialize_format_and_data(GLsizei levels, GLenum internal_format,
		GLsizei width, GLsizei height, const DataT *data){
		initialize_texture(levels,internal_format,width,height);
		set_texture_data(width,height,data);
	}

	template <typename DataT>
	void initialize_format_and_data(GLsizei levels,GLenum internal_format,
									 const texture::image2d_t<DataT>& data){
		initialize_texture(levels,internal_format,data.width(),data.height());
		set_texture_data(data.width(),data.height(),data.get_raw_data());
	}
	template <typename DataT>
	void initialize_format_and_data(GLsizei levels,GLenum internal_format,
									 const math::tensor_t<DataT,2>& data){
		initialize_texture(levels,internal_format,data.shape()[0],data.shape()[1]);
		set_texture_data(data.shape()[0],data.shape()[1],data.raw_data());
	}

	void initialize_texture(GLsizei levels,GLenum sized_internal_format,
							GLsizei width,GLsizei height){
		assert(handle_);
		GL_EXPR(glTextureStorage2D(handle_,levels,sized_internal_format,width,height));
	}

	template <typename DataT>
	void set_texture_data(GLsizei width,GLsizei height,const DataT* data){
		assert(handle_);
		GLint old_row_align;
		GL_EXPR(glGetIntegerv(GL_UNPACK_ALIGNMENT,&old_row_align));
		GL_EXPR(glPixelStorei(GL_UNPACK_ALIGNMENT,
								detail::dt_to_tt<DataT>::row_align));
		GL_EXPR(glTextureSubImage2D(
		  handle_,0,0,0,width,height,
		  detail::dt_to_tt<DataT>::format,
		  detail::dt_to_tt<DataT>::type,data
		  ));
		GL_EXPR(glPixelStorei(GL_UNPACK_ALIGNMENT,old_row_align));
		GL_EXPR(glGenerateTextureMipmap(handle_));
	}

	void set_texture_param(GLenum param_name,GLint param_value){
		assert(handle_);
		GL_EXPR(glTextureParameteri(handle_,param_name,param_value));
	}

	void bind(GLuint texture_unit) const noexcept{
		assert(handle_);
		GL_EXPR(glBindTextureUnit(texture_unit,handle_));
	}
    void bind_image(int binding_point,int level,GLenum access,GLenum format) const noexcept{
        GL_EXPR(glBindImageTexture(binding_point,handle_,level,GL_FALSE,0,access,format));
    }
	void unbind(GLuint texture_unit) const noexcept{
		assert(handle_);
		GL_EXPR(glBindTextureUnit(texture_unit,0));
	}
    void generate_mipmap() const noexcept{
        assert(handle_);
        GL_EXPR(glGenerateTextureMipmap(handle_));
    }
};

class texture3d_t: public gl_object_base_t{
public:
	explicit texture3d_t(bool init_handle = false){
		if(init_handle)
			initialize_handle();
	}

	texture3d_t(texture3d_t&& other) noexcept
	  :gl_object_base_t(other.handle_)
	{
		other.handle_ = 0;
	}

	texture3d_t& operator=(texture3d_t&& other) noexcept{
		destroy();
		std::swap(handle_,other.handle_);
		return *this;
	}

	~texture3d_t(){
		destroy();
	}

	void initialize_handle(){
		assert(!handle_);
		GL_EXPR(glCreateTextures(GL_TEXTURE_3D,1,&handle_));
		if(!handle_)
			throw std::runtime_error("failed to create opengl texture3d object");
	}

	void destroy(){
		if(handle_){
			GL_EXPR(glDeleteTextures(1,&handle_));
			handle_ = 0;
		}
	}

	template <typename DataT>
	void initialize_format_and_data(GLsizei levels, GLenum internal_format,
									 GLsizei width, GLsizei height, GLsizei depth,
									 const DataT *data){
		initialize_texture(levels,internal_format,width,height,depth);
		set_texture_data(width,height,depth,data);
	}

	template <typename DataT>
	void initialize_format_and_data(GLsizei levels,GLenum internal_format,
									 const texture::image3d_t<DataT>& data){
		initialize_texture(levels,internal_format,data.width(),data.height(),data.depth());
		set_texture_data(data.width(),data.height(),data.depth(),data.get_raw_data());
	}
	template <typename DataT>
	void initialize_format_and_data(GLsizei levels,GLenum internal_format,
									 const math::tensor_t<DataT,3>& data){
		initialize_texture(levels,internal_format,data.shape()[0],data.shape()[1],data.shape()[2]);
		set_texture_data(data.shape()[0],data.shape()[1],data.shape()[2],data.raw_data());
	}

	void initialize_texture(GLsizei levels,GLenum internal_format,
							 GLsizei width,GLsizei height,GLsizei depth){
		assert(handle_);
		GL_EXPR(glTextureStorage3D(handle_,levels,internal_format,width,height,depth));
	}

	template <typename DataT>
	void set_texture_data(GLsizei width,GLsizei height,GLsizei depth,const DataT* data){
		assert(handle_);
		GLint old_row_align;
		GL_EXPR(glGetIntegerv(GL_UNPACK_ALIGNMENT,&old_row_align));
		GL_EXPR(glPixelStorei(GL_UNPACK_ALIGNMENT,
								detail::dt_to_tt<DataT>::row_align));
		GL_EXPR(glTextureSubImage3D(
		  handle_,0,0,0,0,width,height,depth,
		  detail::dt_to_tt<DataT>::format,
		  detail::dt_to_tt<DataT>::type,data
		  ));
		GL_EXPR(glPixelStorei(GL_UNPACK_ALIGNMENT,old_row_align));
		GL_EXPR(glGenerateTextureMipmap(handle_));
	}

	void set_texture_param(GLenum param_name,GLint param_value){
		assert(handle_);
		GL_EXPR(glTextureParameteri(handle_,param_name,param_value));
	}

	void bind(GLuint texture_unit) const noexcept{
		assert(handle_);
		GL_EXPR(glBindTextureUnit(texture_unit,handle_));
	}
    void bind_image(int binding_point,int level,GLenum acess,GLenum format) const noexcept{
        GL_EXPR(glBindImageTexture(binding_point,handle_,level,GL_FALSE,0,acess,format));
    }
	void unbind(GLuint texture_unit) const noexcept{
		assert(handle_);
		GL_EXPR(glBindTextureUnit(texture_unit,0));
	}

	void generate_mipmap() const noexcept{
		assert(handle_);
		GL_EXPR(glGenerateTextureMipmap(handle_));
	}
};

}
#include <CGUtils/image/load_image.hpp>
#include <CGUtils/file/raw_file_io.hpp>
#include <cassert>

//todo handle stbi simd ?

namespace {
#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb_image.h"
}

namespace wzz::image{

std::vector<math::byte> load_bytes_from_file(const std::string& filename,int* width,int* height,int* ncomp){
	auto bytes = file::read_raw_file_bytes(filename);
	return load_bytes_from_memory_file(bytes.data(),bytes.size(),width,height,ncomp);
}

math::tensor_t<math::byte,2> load_gray_from_file(const std::string& filename){
	auto bytes = file::read_raw_file_bytes(filename);
	return load_gray_from_memory_file(bytes.data(),bytes.size());
}

math::tensor_t<math::color2b,2> load_gray_alpha_from_file(const std::string& filename){
	auto bytes = file::read_raw_file_bytes(filename);
	return load_gray_alpha_from_memory_file(bytes.data(),bytes.size());
}

math::tensor_t<math::color3b,2> load_rgb_from_file(const std::string& filename){
	auto bytes = file::read_raw_file_bytes(filename);
	return load_rgb_from_memory_file(bytes.data(),bytes.size());
}

math::tensor_t<math::color4b,2> load_rgba_from_file(const std::string& filename){
	auto bytes = file::read_raw_file_bytes(filename);
	return load_rgba_from_memory_file(bytes.data(),bytes.size());
}

math::tensor_t<math::color3f,2> load_rgb_from_hdr_file(const std::string& filename){
	auto bytes = file::read_raw_file_bytes(filename);
	return load_rgb_from_hdr_memory_file(bytes.data(),bytes.size());
}

std::vector<math::byte> load_bytes_from_memory_file(const void* data,size_t bytes_count,int* width,int* height,int* comp){
	if(!data) return {};
	assert(width && height && comp);
	auto bytes = stbi_load_from_memory(static_cast<const stbi_uc*>(data),bytes_count,width,height,comp,0);
	if(!bytes) return {};
	std::vector<math::byte> ret(bytes,bytes + (*width) * (*height) * (*comp));
	stbi_image_free(bytes);
	return ret;
}

math::tensor_t<math::byte,2> load_gray_from_memory_file(const void* data,size_t bytes_count){
	int w,h,comp;
	auto bytes = stbi_load_from_memory(static_cast<const stbi_uc*>(data),bytes_count,&w,&h,&comp,STBI_grey);
	if(!bytes) return {};
	auto ret = math::tensor_t<math::byte,2>::from_linear_array({w,h},static_cast<const math::byte*>(data));
	stbi_image_free(bytes);
	return ret;
}
template <typename T,int C,int STBchannel>
static math::tensor_t<T,2> load_from_memory(const void* data,size_t bytes_count){
	int w,h,comp;
	auto bytes = stbi_load_from_memory(static_cast<const stbi_uc*>(data),bytes_count,&w,&h,&comp,STBchannel);
	if(!bytes) return {};
	auto image = math::tensor_t<T,2>::from_linear_index_func({w,h},[&](int i){
		T ret;
		for(int j = 0; j < C; ++j)
			ret[j] = bytes[i * C + j];
		return ret;
	});
	stbi_image_free(bytes);
	return image;
}
math::tensor_t<math::color2b,2> load_gray_alpha_from_memory_file(const void* data,size_t bytes_count){
	return load_from_memory<math::color2b,2,STBI_grey_alpha>(data,bytes_count);
}

math::tensor_t<math::color3b,2> load_rgb_from_memory_file(const void* data,size_t bytes_count){
	return load_from_memory<math::color3b,3,STBI_rgb>(data,bytes_count);
}

math::tensor_t<math::color4b,2> load_rgba_from_memory_file(const void* data,size_t bytes_count){
	return load_from_memory<math::color4b,4,STBI_rgb_alpha>(data,bytes_count);
}

math::tensor_t<math::color3f,2> load_rgb_from_hdr_memory_file(const void* data,size_t bytes_count){
	int w, h, channels;
	float *d = stbi_loadf_from_memory(static_cast<const stbi_uc *>(data),bytes_count,&w, &h, &channels, STBI_rgb);
	if(!d)
		return math::tensor_t<math::color3f,2>();


	auto ret =  math::tensor_t<math::color3f,2>::from_linear_index_func({w,h},[&](int i){
		math::color3f ret;
		for(int j = 0; j < 3; ++j)
			ret[j] = d[i * 3 + j];
		return ret;
	});
	stbi_image_free(d);
	return ret;
}

}
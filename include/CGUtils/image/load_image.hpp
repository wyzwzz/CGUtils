//
// Created by wyz on 2022/6/10.
//
#pragma once

#include <CGUtils/math.hpp>
#include <string>

namespace wzz::image{


	std::vector<math::byte> load_bytes_from_file(const std::string& filename,int* width,int* height,int* ncomp);

	math::tensor_t<math::byte,2> load_gray_from_file(const std::string& filename);

	math::tensor_t<math::color2b,2> load_gray_alpha_from_file(const std::string& filename);

	math::tensor_t<math::color3b,2> load_rgb_from_file(const std::string& filename);

	math::tensor_t<math::color4b,2> load_rgba_from_file(const std::string& filename);

	math::tensor_t<math::color3f,2> load_rgb_from_hdr_file(const std::string& filename);

	std::vector<math::byte> load_bytes_from_memory_file(const void* data,size_t bytes_count,int* width,int* height,int* comp);

	math::tensor_t<math::byte,2> load_gray_from_memory_file(const void* data,size_t bytes_count);

	math::tensor_t<math::color2b,2> load_gray_alpha_from_memory_file(const void* data,size_t bytes_count);

	math::tensor_t<math::color3b,2> load_rgb_from_memory_file(const void* data,size_t bytes_count);

	math::tensor_t<math::color4b,2> load_rgba_from_memory_file(const void* data,size_t bytes_count);

	math::tensor_t<math::color3f,2> load_rgb_from_hdr_memory_file(const void* data,size_t bytes_count);

}
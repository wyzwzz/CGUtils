//
// Created by wyz on 2022/6/10.
//
#pragma once
#include <CGUtils/math.hpp>
#include <string>

namespace wzz::image{
	void save_rgb_to_png_file(const std::string& filename,const math::tensor_t<math::color3b,2>& data);
}
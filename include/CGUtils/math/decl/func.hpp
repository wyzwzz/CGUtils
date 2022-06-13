#pragma once

#include <atomic>
#include <cmath>

#include "./vec2.hpp"
#include "./vec3.hpp"
#include "./vec4.hpp"
#include "./color2.hpp"
#include "./color3.hpp"
#include "./color4.hpp"

WZZ_MATH_BEGIN

template <typename T,std::enable_if<!std::is_floating_point_v<T>>>
bool is_power_of_2(T v) noexcept{
	return v > 0 && !(v & (v - 1));
}

WZZ_MATH_END


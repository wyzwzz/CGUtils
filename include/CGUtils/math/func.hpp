#pragma once

#include <atomic>
#include <cmath>

#include "./vec2.hpp"
#include "./vec3.hpp"
#include "./vec4.hpp"
#include "color2.hpp"
#include "./color3.hpp"
#include "./color4.hpp"

WZZ_MATH_BEGIN

template <typename T,typename std::enable_if_t<!std::is_floating_point_v<T>>>
bool is_power_of_2(T v) noexcept{
	return v > 0 && !(v & (v - 1));
}

template<typename T>
struct min_value_t{
	constexpr T operator()(const T& l,const T& r) const{
		return l < r ? l : r;
	}
};

template<typename T>
struct max_value_t{
	max_value_t() = default;
	constexpr T operator()(const T& l,const T& r) const{
		return l < r ? r : l;
	}
};


template<typename T>
struct avg_value_t{
	avg_value_t() = default;
	constexpr T operator()(const T& l,const T& r) const{
		return (l + r) * static_cast<T>(0.5);
	}
};

WZZ_MATH_END


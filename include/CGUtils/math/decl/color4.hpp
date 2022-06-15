#pragma once
#include "common.hpp"
WZZ_MATH_BEGIN

template <typename T>
class tcolor4{
public:
	T r, g, b, a;

	T& operator[](int idx) noexcept { return *(&r + idx);}
	const T& operator[](int idx) const noexcept { return *(&r + idx);}
};


using color4f = tcolor4<float>;
using color4d = tcolor4<double>;
using color4b = tcolor4<unsigned char>;
WZZ_MATH_END
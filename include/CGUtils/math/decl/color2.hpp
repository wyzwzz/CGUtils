#pragma once

#include "CGUtils/math/decl/common.hpp"

WZZ_MATH_BEGIN
template <typename T>
class tcolor2{
public:
	T r, g;

	T& operator[](int idx) noexcept { return *(&r + idx);}
	const T& operator[](int idx) const noexcept { return *(&r + idx);}
};

using color2f = tcolor2<float>;
using color2d = tcolor2<double>;
using color2b = tcolor2<unsigned char>;

WZZ_MATH_END
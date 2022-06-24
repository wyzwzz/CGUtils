#pragma once
#include "common.hpp"

WZZ_MATH_BEGIN
template <typename T>
class tcolor3{
public:
	T r, g, b;
	tcolor3();
	explicit tcolor3(T v) noexcept;
	tcolor3(T r, T g, T b) noexcept;

	T& operator[](int idx) noexcept { return *(&r + idx);}
	const T& operator[](int idx) const noexcept { return *(&r + idx);}
};


using color3f = tcolor3<float>;
using color3d = tcolor3<double>;
using color3b = tcolor3<unsigned char>;

WZZ_MATH_END
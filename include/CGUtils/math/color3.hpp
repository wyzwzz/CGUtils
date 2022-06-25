#pragma once

#include "common.hpp"

WZZ_MATH_BEGIN

template <typename T>
class tcolor3{
public:
	T r, g, b;

	tcolor3() noexcept;
	tcolor3(T r, T g, T b) noexcept;
	explicit tcolor3(T v) noexcept;
	explicit tcolor3(uninitialized_t) noexcept;
	explicit tcolor3(const tvec3<T>& v) noexcept;

	tcolor3 clamp(T min_v, T max_v) const noexcept;

	tcolor3 saturate() const noexcept;

	bool is_finite() const noexcept;

	bool is_valid() const noexcept;

	bool is_black() const noexcept;

	T lum() const noexcept;

	template <typename F>
	auto map(F&& f) const noexcept;

	T& operator[](int idx) noexcept ;
	const T& operator[](int idx) const noexcept ;

	tcolor3& operator+=(const tcolor3& rhs) noexcept;
	tcolor3& operator-=(const tcolor3& rhs) noexcept;
	tcolor3& operator*=(const tcolor3& rhs) noexcept;
	tcolor3& operator/=(const tcolor3& rhs) noexcept;

	tcolor3& operator+=(T rhs) noexcept;
	tcolor3& operator-=(T rhs) noexcept;
	tcolor3& operator*=(T rhs) noexcept;
	tcolor3& operator/=(T rhs) noexcept;

#define SWIZZLE2(A, B)       tcolor2<T> A##B()       const { return tcolor2<T>(A, B); }
#define SWIZZLE3(A, B, C)    tcolor3<T> A##B##C()    const { return tcolor3<T>(A, B, C); }
	SWIZZLE2(r, r) SWIZZLE2(r, g) SWIZZLE2(r, b) SWIZZLE2(g, r) SWIZZLE2(g, g) SWIZZLE2(g, b) SWIZZLE2(b, r) SWIZZLE2(b, g) SWIZZLE2(b, b)
	SWIZZLE3(r, r, r) SWIZZLE3(r, r, g) SWIZZLE3(r, r, b) SWIZZLE3(r, g, r) SWIZZLE3(r, g, g) SWIZZLE3(r, g, b) SWIZZLE3(r, b, r) SWIZZLE3(r, b, g) SWIZZLE3(r, b, b)
	SWIZZLE3(g, r, r) SWIZZLE3(g, r, g) SWIZZLE3(g, r, b) SWIZZLE3(g, g, r) SWIZZLE3(g, g, g) SWIZZLE3(g, g, b) SWIZZLE3(g, b, r) SWIZZLE3(g, b, g) SWIZZLE3(g, b, b)
	SWIZZLE3(b, r, r) SWIZZLE3(b, r, g) SWIZZLE3(b, r, b) SWIZZLE3(b, g, r) SWIZZLE3(b, g, g) SWIZZLE3(b, g, b) SWIZZLE3(b, b, r) SWIZZLE3(b, b, g) SWIZZLE3(b, b, b)
#undef SWIZZLE3
};

template <typename T>
tcolor3<T> operator-(const tcolor3<T>& color) noexcept;

template <typename T>
tcolor3<T> operator+(const tcolor3<T>& lhs,const tcolor3<T>& rhs) noexcept;

template <typename T>
tcolor3<T> operator-(const tcolor3<T>& lhs,const tcolor3<T>& rhs) noexcept;

template <typename T>
tcolor3<T> operator*(const tcolor3<T>& lhs,const tcolor3<T>& rhs) noexcept;

template <typename T>
tcolor3<T> operator/(const tcolor3<T>& lhs,const tcolor3<T>& rhs) noexcept;

template <typename T>
tcolor3<T> operator+(const tcolor3<T>& lhs,T rhs) noexcept;

template <typename T>
tcolor3<T> operator-(const tcolor3<T>& lhs,T rhs) noexcept;

template <typename T>
tcolor3<T> operator*(const tcolor3<T>& lhs,T rhs) noexcept;

template <typename T>
tcolor3<T> operator/(const tcolor3<T>& lhs,T rhs) noexcept;

template<typename T>
tcolor3<T> operator+(T lhs, const tcolor3<T> &rhs) noexcept;

template<typename T>
tcolor3<T> operator-(T lhs, const tcolor3<T> &rhs) noexcept;

template<typename T>
tcolor3<T> operator*(T lhs, const tcolor3<T> &rhs) noexcept;

template<typename T>
bool operator==(const tcolor3<T> &lhs, const tcolor3<T> &rhs) noexcept;

template<typename T>
bool operator!=(const tcolor3<T> &lhs, const tcolor3<T> &rhs) noexcept;

using color3f = tcolor3<float>;
using color3d = tcolor3<double>;
using color3b = tcolor3<unsigned char>;

template <typename T,typename = std::enable_if_t<std::is_floating_point_v<T>>>
color3b to_color3b(const tcolor3<T>& color) noexcept;

template<typename T,typename = std::enable_if_t<std::is_floating_point_v<T>>>
tcolor3<T> from_color3b(const color3b& color) noexcept;

WZZ_MATH_END

namespace std{

template <typename T>
struct hash<wzz::math::tcolor3<T>>{
	size_t operator()(const wzz::math::tcolor3<T>& color) const noexcept{
		return wzz::misc::hash(color.x,color.y,color.z);
	}
};

}
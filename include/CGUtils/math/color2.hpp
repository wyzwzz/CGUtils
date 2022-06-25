#pragma once

#include "common.hpp"

WZZ_MATH_BEGIN

template <typename T>
class tcolor2{
public:
	T r, g;

	tcolor2() noexcept;
	tcolor2(T r, T g) noexcept;
	explicit tcolor2(T v) noexcept;
	explicit tcolor2(uninitialized_t) noexcept;
	explicit tcolor2(const tvec2<T>& v) noexcept;

	tcolor2 clamp(T min_v, T max_v) const noexcept;

	tcolor2 saturate() const noexcept;

	bool is_finite() const noexcept;

	bool is_valid() const noexcept;

	bool is_black() const noexcept;

	template <typename F>
	auto map(F&& f) const noexcept;

	T& operator[](int idx) noexcept ;
	const T& operator[](int idx) const noexcept ;

	tcolor2& operator+=(const tcolor2& rhs) noexcept;
	tcolor2& operator-=(const tcolor2& rhs) noexcept;
	tcolor2& operator*=(const tcolor2& rhs) noexcept;
	tcolor2& operator/=(const tcolor2& rhs) noexcept;

	tcolor2& operator+=(T rhs) noexcept;
	tcolor2& operator-=(T rhs) noexcept;
	tcolor2& operator*=(T rhs) noexcept;
	tcolor2& operator/=(T rhs) noexcept;

#define SWIZZLE2(A, B)       tcolor2<T> A##B()       const { return tcolor2<T>(A, B); }
	SWIZZLE2(r, g) SWIZZLE2(r, r) SWIZZLE2(g, r) SWIZZLE2(g, g)
#undef SWIZZLE2
};

template <typename T>
tcolor2<T> operator-(const tcolor2<T>& color) noexcept;

template <typename T>
tcolor2<T> operator+(const tcolor2<T>& lhs,const tcolor2<T>& rhs) noexcept;

template <typename T>
tcolor2<T> operator-(const tcolor2<T>& lhs,const tcolor2<T>& rhs) noexcept;

template <typename T>
tcolor2<T> operator*(const tcolor2<T>& lhs,const tcolor2<T>& rhs) noexcept;

template <typename T>
tcolor2<T> operator/(const tcolor2<T>& lhs,const tcolor2<T>& rhs) noexcept;

template <typename T>
tcolor2<T> operator+(const tcolor2<T>& lhs,T rhs) noexcept;

template <typename T>
tcolor2<T> operator-(const tcolor2<T>& lhs,T rhs) noexcept;

template <typename T>
tcolor2<T> operator*(const tcolor2<T>& lhs,T rhs) noexcept;

template <typename T>
tcolor2<T> operator/(const tcolor2<T>& lhs,T rhs) noexcept;

template<typename T>
tcolor2<T> operator+(T lhs, const tcolor2<T> &rhs) noexcept;

template<typename T>
tcolor2<T> operator-(T lhs, const tcolor2<T> &rhs) noexcept;

template<typename T>
tcolor2<T> operator*(T lhs, const tcolor2<T> &rhs) noexcept;

template<typename T>
bool operator==(const tcolor2<T> &lhs, const tcolor2<T> &rhs) noexcept;

template<typename T>
bool operator!=(const tcolor2<T> &lhs, const tcolor2<T> &rhs) noexcept;

using color2f = tcolor2<float>;
using color2d = tcolor2<double>;
using color2b = tcolor2<unsigned char>;

template <typename T,typename = std::enable_if_t<std::is_floating_point_v<T>>>
color2b to_color2b(const tcolor2<T>& color) noexcept;

template<typename T,typename = std::enable_if_t<std::is_floating_point_v<T>>>
tcolor2<T> from_color2b(const color2b& color) noexcept;

WZZ_MATH_END

namespace std{

template <typename T>
struct hash<wzz::math::tcolor2<T>>{
	size_t operator()(const wzz::math::tcolor2<T>& color) const noexcept{
		return wzz::misc::hash(color.x,color.y);
	}
};

}
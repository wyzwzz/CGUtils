#pragma once

#include "common.hpp"

WZZ_MATH_BEGIN

template <typename T>
class tcolor4{
public:
	T r, g, b, a;

	tcolor4() noexcept;
	tcolor4(T r, T g, T b, T a) noexcept;
	explicit tcolor4(T v) noexcept;
	explicit tcolor4(uninitialized_t) noexcept;
	explicit tcolor4(const tvec4<T>& v) noexcept;

	tcolor4 clamp(T min_v, T max_v) const noexcept;

	tcolor4 saturate() const noexcept;

	bool is_finite() const noexcept;

	bool is_valid() const noexcept;

	bool is_black() const noexcept;

	template <typename F>
	auto map(F&& f) const noexcept;

	T& operator[](int idx) noexcept ;
	const T& operator[](int idx) const noexcept ;

	tcolor4& operator+=(const tcolor4& rhs) noexcept;
	tcolor4& operator-=(const tcolor4& rhs) noexcept;
	tcolor4& operator*=(const tcolor4& rhs) noexcept;
	tcolor4& operator/=(const tcolor4& rhs) noexcept;

	tcolor4& operator+=(T rhs) noexcept;
	tcolor4& operator-=(T rhs) noexcept;
	tcolor4& operator*=(T rhs) noexcept;
	tcolor4& operator/=(T rhs) noexcept;

#include "./impl/swizzle_color4.inl"
};

template <typename T>
tcolor4<T> operator-(const tcolor4<T>& color) noexcept;

template <typename T>
tcolor4<T> operator+(const tcolor4<T>& lhs,const tcolor4<T>& rhs) noexcept;

template <typename T>
tcolor4<T> operator-(const tcolor4<T>& lhs,const tcolor4<T>& rhs) noexcept;

template <typename T>
tcolor4<T> operator*(const tcolor4<T>& lhs,const tcolor4<T>& rhs) noexcept;

template <typename T>
tcolor4<T> operator/(const tcolor4<T>& lhs,const tcolor4<T>& rhs) noexcept;

template <typename T>
tcolor4<T> operator+(const tcolor4<T>& lhs,T rhs) noexcept;

template <typename T>
tcolor4<T> operator-(const tcolor4<T>& lhs,T rhs) noexcept;

template <typename T>
tcolor4<T> operator*(const tcolor4<T>& lhs,T rhs) noexcept;

template <typename T>
tcolor4<T> operator/(const tcolor4<T>& lhs,T rhs) noexcept;

template<typename T>
tcolor4<T> operator+(T lhs, const tcolor4<T> &rhs) noexcept;

template<typename T>
tcolor4<T> operator-(T lhs, const tcolor4<T> &rhs) noexcept;

template<typename T>
tcolor4<T> operator*(T lhs, const tcolor4<T> &rhs) noexcept;

template<typename T>
bool operator==(const tcolor4<T> &lhs, const tcolor4<T> &rhs) noexcept;

template<typename T>
bool operator!=(const tcolor4<T> &lhs, const tcolor4<T> &rhs) noexcept;

using color4f = tcolor4<float>;
using color4d = tcolor4<double>;
using color4b = tcolor4<unsigned char>;

template <typename T,typename = std::enable_if_t<std::is_floating_point_v<T>>>
color4b to_color4b(const tcolor4<T>& color) noexcept;

template<typename T,typename = std::enable_if_t<std::is_floating_point_v<T>>>
tcolor4<T> from_color4b(const color4b& color) noexcept;

WZZ_MATH_END

namespace std{

template <typename T>
struct hash<wzz::math::tcolor4<T>>{
	size_t operator()(const wzz::math::tcolor4<T>& color) const noexcept{
		return wzz::misc::hash(color.x,color.y,color.z,color.w);
	}
};

}
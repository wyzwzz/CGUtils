#pragma once

#include <ostream>
#include "common.hpp"
#include "CGUtils/misc/hash.hpp"

WZZ_MATH_BEGIN

template <typename T>
class tvec4{
  public:
    using self_t = tvec4<T>;

    T x,y,z,w;

    tvec4() noexcept;
    tvec4(T x, T y, T z, T w) noexcept;
    tvec4( T val) noexcept;
    tvec4(uninitialized_t) noexcept;
    tvec4(const self_t& v) noexcept;
    tvec4(const tvec<T,4>& v) noexcept;
	tvec4(tvec3<T> x, T y) noexcept;

    bool is_zero() const noexcept;

    auto length() const noexcept;

    auto length_square() const noexcept;

    void normalize() noexcept;

    self_t normalized() const noexcept;

	template <typename F>
	auto map(F&& f) const noexcept;

	template <typename U>
	auto convert_to() const noexcept;

    T& operator[](int idx) noexcept;
    const T& operator[](int idx) const noexcept;

    self_t& operator+=(const self_t& rhs) noexcept;
    self_t& operator-=(const self_t& rhs) noexcept;
    self_t& operator*=(const self_t& rhs) noexcept;
    self_t& operator/=(const self_t& rhs) noexcept;

    self_t& operator+=(T rhs) noexcept;
    self_t& operator-=(T rhs) noexcept;
    self_t& operator*=(T rhs) noexcept;
    self_t& operator/=(T rhs) noexcept;

    tvec3<T> homogenize() const noexcept;

#include "CGUtils/math/impl/swizzle_vec4.inl"
};

template <typename T>
auto operator-(const tvec4<T>& vec) noexcept;

template <typename T>
auto operator+(const tvec4<T>& lhs,const tvec4<T>& rhs) noexcept;

template <typename T>
auto operator-(const tvec4<T>& lhs,const tvec4<T>& rhs) noexcept;

template <typename T>
auto operator*(const tvec4<T>& lhs,const tvec4<T>& rhs) noexcept;

template <typename T>
auto operator/(const tvec4<T>& lhs,const tvec4<T>& rhs) noexcept;

template <typename T>
auto operator+(const tvec4<T>& lhs, T rhs) noexcept;

template <typename T>
auto operator-(const tvec4<T>& lhs, T rhs) noexcept;

template <typename T>
auto operator*(const tvec4<T>& lhs, T rhs) noexcept;

template <typename T>
auto operator/(const tvec4<T>& lhs, T rhs) noexcept;

template <typename T>
auto operator+(T lhs, const tvec4<T>& rhs) noexcept;

template <typename T>
auto operator*(T lhs, const tvec4<T>& rhs) noexcept;

template <typename T>
bool operator==(const tvec4<T>& lhs, const tvec4<T>& rhs) noexcept;

template <typename T>
bool operator!=(const tvec4<T>& lhs, const tvec4<T>& rhs) noexcept;

template <typename T>
bool operator<(const tvec4<T>& lhs,const tvec4<T>& rhs) noexcept;

template <typename T>
bool operator>(const tvec4<T>& lhs,const tvec4<T>& rhs) noexcept;

template <typename T>
auto dot(const tvec4<T>& lhs, const tvec4<T>& rhs) noexcept;

template <typename T>
auto cos(const tvec4<T>& lhs, const tvec4<T>& rhs) noexcept;

template <typename T>
std::ostream& operator<<(std::ostream& out, const tvec4<T>& vec);

using vec4f = tvec4<float>;
using vec4d = tvec4<double>;
using vec4i = tvec4<int>;
using vec4b = tvec4<unsigned char>;

WZZ_MATH_END

namespace std{

template <typename T>
struct hash<wzz::math::tvec4<T>>{
    size_t operator()(const wzz::math::tvec4<T>& vec) const noexcept{
        return wzz::misc::hash(vec.x,vec.y,vec.z,vec.w);
    }
};

}

//
// Created by wyz on 2022/2/14.
//
#pragma once
#include <ostream>
#include "common.hpp"
#include "../../misc/hash.hpp"
WYZ_MATH_BEGIN

template <typename T>
class tvec3{
  public:
    using self_t = tvec3<T>;

    T x,y,z;

    tvec3() noexcept;
    tvec3(T x, T y, T z) noexcept;
    tvec3( T val) noexcept;
    tvec3(uninitialized_t) noexcept;
    tvec3(const self_t& v) noexcept;
    tvec3(const tvec<T,3>& v) noexcept;

    bool is_zero() const noexcept;

    auto length() const noexcept;

    auto length_square() const noexcept;

    void normalize() noexcept;

    self_t normalized() const noexcept;

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

#include "../impl/swizzle_vec3.inl"
};

template <typename T>
auto operator-(const tvec3<T>& vec) noexcept;

template <typename T>
auto operator+(const tvec3<T>& lhs,const tvec3<T>& rhs) noexcept;

template <typename T>
auto operator-(const tvec3<T>& lhs,const tvec3<T>& rhs) noexcept;

template <typename T>
auto operator*(const tvec3<T>& lhs,const tvec3<T>& rhs) noexcept;

template <typename T>
auto operator/(const tvec3<T>& lhs,const tvec3<T>& rhs) noexcept;

template <typename T>
auto operator+(const tvec3<T>& lhs, T rhs) noexcept;

template <typename T>
auto operator-(const tvec3<T>& lhs, T rhs) noexcept;

template <typename T>
auto operator*(const tvec3<T>& lhs, T rhs) noexcept;

template <typename T>
auto operator/(const tvec3<T>& lhs, T rhs) noexcept;

template <typename T>
auto operator+(T lhs, const tvec3<T>& rhs) noexcept;

template <typename T>
auto operator*(T lhs, const tvec3<T>& rhs) noexcept;

template <typename T>
bool operator==(const tvec3<T>& lhs, const tvec3<T>& rhs) noexcept;

template <typename T>
bool operator!=(const tvec3<T>& lhs, const tvec3<T>& rhs) noexcept;

template <typename T>
bool operator<(const tvec3<T>& lhs,const tvec3<T>& rhs) noexcept;

template <typename T>
bool operator>(const tvec3<T>& lhs,const tvec3<T>& rhs) noexcept;

template <typename T>
auto dot(const tvec3<T>& lhs, const tvec3<T>& rhs) noexcept;

template <typename T>
auto cross(const tvec3<T>& lhs, const tvec3<T>& rhs) noexcept;

template <typename T>
auto cos(const tvec3<T>& lhs, const tvec3<T>& rhs) noexcept;

template <typename T>
std::ostream& operator<<(std::ostream& out, const tvec3<T>& vec);

using vec3f = tvec3<float>;
using vec3d = tvec3<double>;
using vec3i = tvec3<int>;
using vec3b = tvec3<unsigned char>;

WYZ_MATH_END

namespace std{

template <typename T>
struct hash<wyz::math::tvec3<T>>{
    size_t operator()(const wyz::math::tvec3<T>& vec) const noexcept{
        return wyz::misc::hash(vec.x,vec.y,vec.z);
    }
};

}

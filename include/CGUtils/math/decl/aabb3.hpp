//
// Created by wyz on 2022/2/14.
//
#pragma once

#include "vec3.hpp"

WZZ_MATH_BEGIN

template <typename T>
class taabb3{
  public:
    using self_t = taabb3<T>;

    tvec3<T> low;
    tvec3<T> high;

    taabb3() noexcept;
    taabb3(uninitialized_t) noexcept;
    taabb3(const tvec3<T>& low,const tvec3<T>& high) noexcept;
    taabb3(const self_t&) = default;

    bool is_valid() const noexcept;

    bool intersect(const self_t& other) const noexcept;

    bool contains(const self_t& other) const noexcept;

    bool contains(const tvec3<T>& other) const noexcept;

    bool within(const self_t& other) const noexcept;

    self_t& operator|=(const tvec3<T>& rhs) noexcept;

    self_t& operator|=(const taabb3<T>& rhs) noexcept;

    self_t& operator&=(const taabb3<T>& rhs) noexcept;

    self_t& operator*=(const tvec3<T>& rhs) noexcept;
};

template <typename T>
taabb3<T> operator|(const taabb3<T>& a,const taabb3<T>& b);

template <typename T>
taabb3<T> operator|(const taabb3<T>& a,const tvec3<T>& b);

template <typename T>
taabb3<T> operator&(const taabb3<T>& a,const taabb3<T>& b);

template <typename T>
taabb3<T> operator*(const taabb3<T>& a,const tvec3<T>& b);

using aabb3f = taabb3<float>;
using aabb3d = taabb3<double>;
using aabb3i = taabb3<int>;

WZZ_MATH_END
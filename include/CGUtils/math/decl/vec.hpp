//
// Created by wyz on 2022/2/14.
//
#pragma once

#include <tuple>
#include "common.hpp"

WZZ_MATH_BEGIN

template <typename T, int D>
class tvec{
public:
    static_assert(D > 0, "dimension of tvec must more than 0");
    using self_t = tvec<T,D>;

    T data[D];

    tvec() noexcept;
    explicit tvec(T val) noexcept;
    explicit tvec(uninitialized_t) noexcept;
    template<typename... As,typename = std::enable_if_t<(D > 1 && sizeof...(As) == D)>>
    tvec(As&&... args) noexcept;
    T& operator[](int idx) noexcept;
    const T& operator[](int idx) const noexcept;
};

WZZ_MATH_END
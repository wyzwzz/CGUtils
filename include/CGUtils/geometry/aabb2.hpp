#pragma once

#include "CGUtils/math/vec2.hpp"

namespace wzz::geometry {
    using namespace wzz::math;
    template<typename T>
    class taabb2 {
    public:
        using self_t = taabb2<T>;

        tvec2 <T> low;
        tvec2 <T> high;

        taabb2() noexcept;

        taabb2(uninitialized_t) noexcept;

        taabb2(const tvec2 <T> &low, const tvec2 <T> &high) noexcept;

        taabb2(const self_t &) = default;

        bool is_valid() const noexcept;

        bool intersect(const self_t &other) const noexcept;

        bool contains(const self_t &other) const noexcept;

        bool contains(const tvec2 <T> &other) const noexcept;

        bool within(const self_t &other) const noexcept;

        self_t &operator|=(const taabb2<T> &rhs) noexcept;

        self_t &operator|=(const tvec2 <T> &rhs) noexcept;

        self_t &operator&=(const taabb2<T> &rhs) noexcept;

        self_t &operator*=(const tvec2 <T> &rhs) noexcept;
    };

    template<typename T>
    taabb2<T> operator|(const taabb2<T> &a, const taabb2<T> &b);

    template<typename T>
    taabb2<T> operator|(const taabb2<T> &a, const tvec2 <T> &b);

    template<typename T>
    taabb2<T> operator&(const taabb2<T> &a, const taabb2<T> &b);

    template<typename T>
    taabb2<T> operator*(const taabb2<T> &a, const tvec2 <T> &b);

    using aabb2f = taabb2<float>;
    using aabb2d = taabb2<double>;
    using aabb2i = taabb2<int>;

}
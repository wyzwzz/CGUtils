#pragma once

#include "CGUtils/geometry/aabb3.hpp"

namespace wzz::geometry {

    template<typename T>
    taabb3<T>::taabb3() noexcept
    :low(std::numeric_limits<float>::max()),
    high(-std::numeric_limits<float>::max())
    {

    }

    template<typename T>
    taabb3<T>::taabb3(uninitialized_t) noexcept
            :low(uninitialized_t{}), high(uninitialized_t{}) {

    }

    template<typename T>
    taabb3<T>::taabb3(const tvec3<T> &low, const tvec3<T> &high) noexcept
            :low(low), high(high) {

    }

    template<typename T>
    bool taabb3<T>::is_valid() const noexcept {
        return low.x <= high.x && low.y <= high.y && low.z <= high.z;
    }

    template<typename T>
    bool taabb3<T>::intersect(const self_t &other) const noexcept {
        return (*this | other).is_valid();
    }

    template<typename T>
    bool taabb3<T>::contains(const self_t &other) const noexcept {
        return other.within(*this);
    }

    template<typename T>
    bool taabb3<T>::contains(const tvec3<T> &other) const noexcept {
        return low.x <= other.x && low.y <= other.y && low.z <= other.z
               && high.x >= other.x && high.y >= other.y && high.z >= other.z;
    }

    template<typename T>
    bool taabb3<T>::within(const self_t &other) const noexcept {
        return low.x <= other.low.x && low.y <= other.low.y && low.z <= other.low.z
               && high.x >= other.high.x && high.y >= other.high.y && high.z >= other.high.z;
    }

    template<typename T>
    taabb3<T> &taabb3<T>::operator|=(const taabb3<T> &rhs) noexcept {
        return *this = *this | rhs;
    }

    template<typename T>
    taabb3<T> &taabb3<T>::operator|=(const tvec3<T> &rhs) noexcept {
        return *this = *this | rhs;
    }

    template<typename T>
    taabb3<T> &taabb3<T>::operator&=(const taabb3<T> &rhs) noexcept {
        return *this = *this & rhs;
    }

    template<typename T>
    taabb3<T> &taabb3<T>::operator*=(const tvec3<T> &rhs) noexcept {
        low *= rhs;
        high *= rhs;
        return *this;
    }

    template<typename T>
    int taabb3<T>::maximum_extent() const noexcept {
        auto d = diagonal();
        if(d.x > d.y && d.x > d.z)
            return 0;
        else if(d.y > d.z)
            return 1;
        else
            return 2;
    }

    template<typename T>
    tvec3<T> taabb3<T>::offset(const tvec3<T> &p) const noexcept {
        auto o = p - low;
        auto d = diagonal();
        assert(d.x >= 0 && d.y >= 0 && d.z >= 0);
        return o / d;
    }

    template<typename T>
    T taabb3<T>::surface_area() const noexcept {
        auto d = diagonal();
        return 2 * (d.x * d.y + d.y * d.z + d.z * d.x);
    }

    template<typename T>
    tvec3<T> &taabb3<T>::operator[](int id) noexcept {
        assert(id == 0 || id == 1);
        return id == 0 ? low : high;
    }

    template<typename T>
    const tvec3<T> &taabb3<T>::operator[](int id) const noexcept {
        assert(id == 0 || id == 1);
        return id == 0 ? low : high;
    }

    template<typename T>
    tvec3<T> taabb3<T>::diagonal() const noexcept {
        return wzz::math::tvec3<T>(high - low);
    }

    template<typename T>
    T taabb3<T>::volume() const noexcept {
        auto d = diagonal();
        return d.x * d.y * d.z;
    }

    template<typename T>
    std::pair<tvec3<T>, T> taabb3<T>::bounding_sphere() const noexcept {
        auto center = (low + high) * T(0.5);
        return std::make_pair(center,is_valid() ? (high - center).length() : 0);
    }

    template<typename T>
    taabb3<T> operator|(const taabb3<T> &a, const taabb3<T> &b) {
        return taabb3<T>(
                tvec3<T>((std::min)(a.low.x, b.low.x), (std::min)(a.low.y, b.low.y), (std::min)(a.low.z, b.low.z)),
                tvec3<T>((std::max)(a.high.x, b.high.x), (std::max)(a.high.y, b.high.y), (std::max)(a.high.z, b.high.z))
        );
    }

    template<typename T>
    taabb3<T> operator|(const taabb3<T> &a, const tvec3<T> &b) {
        return taabb3<T>(
                tvec3<T>((std::min)(a.low.x, b.x), (std::min)(a.low.y, b.y), (std::min)(a.low.z, b.z)),
                tvec3<T>((std::max)(a.high.x, b.x), (std::max)(a.high.y, b.y), (std::max)(a.high.z, b.z))
        );
    }

    template<typename T>
    taabb3<T> operator&(const taabb3<T> &a, const taabb3<T> &b) {
        return taabb3<T>(
                tvec3<T>((std::max)(a.low.x, b.low.x), (std::max)(a.low.y, b.low.y), (std::max)(a.low.z, b.low.z)),
                tvec3<T>((std::min)(a.high.x, b.high.x), (std::min)(a.high.y, b.high.y), (std::min)(a.high.z, b.high.z))
        );
    }

    template<typename T>
    taabb3<T> operator*(const taabb3<T> &a, const tvec3<T> &b) {
        return taabb3<T>(
                a.low * b,
                a.high * b
        );
    }

}
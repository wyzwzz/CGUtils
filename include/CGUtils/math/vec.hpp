#pragma once

#include <ostream>
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

	bool is_zero() const noexcept;

	auto length() const noexcept;
	auto length_squared() const noexcept;

	void normalize() noexcept;
	self_t normalized() const noexcept;

	auto sum() const noexcept;
	auto product() const noexcept;

	template <typename F>
	auto map(F&& f) const noexcept;

	template <typename U>
	auto convert_to() const noexcept;

	bool operator!() const noexcept;

	T& operator[](int idx) noexcept;
    const T& operator[](int idx) const noexcept;

	self_t &operator+=(const self_t &rhs) noexcept;
	self_t &operator-=(const self_t &rhs) noexcept;
	self_t &operator*=(const self_t &rhs) noexcept;
	self_t &operator/=(const self_t &rhs) noexcept;

	self_t &operator+=(T rhs) noexcept;
	self_t &operator-=(T rhs) noexcept;
	self_t &operator*=(T rhs) noexcept;
	self_t &operator/=(T rhs) noexcept;
};

template<typename T, int D>
tvec<T, D> operator-(const tvec<T, D> &vec) noexcept;

template<typename T, int D>
tvec<T, D> operator+(const tvec<T, D> &lhs, const tvec<T, D> &rhs) noexcept;

template<typename T, int D>
tvec<T, D> operator-(const tvec<T, D> &lhs, const tvec<T, D> &rhs) noexcept;

template<typename T, int D>
tvec<T, D> operator*(const tvec<T, D> &lhs, const tvec<T, D> &rhs) noexcept;

template<typename T, int D>
tvec<T, D> operator/(const tvec<T, D> &lhs, const tvec<T, D> &rhs) noexcept;

template<typename T, int D>
tvec<T, D> operator+(const tvec<T, D> &lhs, T rhs) noexcept;

template<typename T, int D>
tvec<T, D> operator-(const tvec<T, D> &lhs, T rhs) noexcept;

template<typename T, int D>
tvec<T, D> operator*(const tvec<T, D> &lhs, T rhs) noexcept;

template<typename T, int D>
tvec<T, D> operator/(const tvec<T, D> &lhs, T rhs) noexcept;

template<typename T, int D>
tvec<T, D> operator+(T lhs, const tvec<T, D> &rhs) noexcept;

template<typename T, int D>
tvec<T, D> operator*(T lhs, const tvec<T, D> &rhs) noexcept;

template<typename T, int D>
bool operator==(const tvec<T, D> &lhs, const tvec<T, D> &rhs) noexcept;

template<typename T, int D>
bool operator!=(const tvec<T, D> &lhs, const tvec<T, D> &rhs) noexcept;

template<typename T, int D>
auto dot(const tvec<T, D> &lhs, const tvec<T, D> &rhs) noexcept;

template<typename T, int D>
auto cos(const tvec<T, D> &lhs, const tvec<T, D> &rhs) noexcept;

template<typename T, int D>
std::ostream &operator<<(std::ostream &out, const tvec<T, D> &vec);

template<typename T, int D>
using vec = tvec<T, D>;

WZZ_MATH_END
#pragma once
#include <cmath>
WZZ_MATH_BEGIN

template <typename T>
tvec2<T>::tvec2() noexcept
    :tvec2(T(0))
{

}

template <typename T>
tvec2<T>::tvec2(T x, T y) noexcept
    :x(x),y(y)
{

}

template <typename T>
tvec2<T>::tvec2(T val) noexcept
    :x(val),y(val)
{

}

template <typename T>
tvec2<T>::tvec2(uninitialized_t) noexcept{}

template <typename T>
tvec2<T>::tvec2(const self_t& v) noexcept
    :x(v.x),y(v.y)
{

}

template <typename T>
tvec2<T>::tvec2(const tvec<T,2>& v) noexcept
    :x(v[0]),y(v[1])
{

}

template <typename T>
bool tvec2<T>::is_zero() const noexcept
{
    return x==T(0) && y==T(0);
}

template <typename T>
auto tvec2<T>::length() const noexcept
{
    return std::sqrt(length_square());
}

template <typename T>
auto tvec2<T>::length_square() const noexcept
{
    return x*x + y*y;
}

template <typename T>
void tvec2<T>::normalize() noexcept{
    static_assert(std::is_floating_point_v<T>, "must be float point");
    this->operator/=(length());
}

template <typename T>
tvec2<T> tvec2<T>::normalized() const noexcept
{
    static_assert(std::is_floating_point_v<T>, "must be float point");
    return *this / length();
}

template <typename T>
T& tvec2<T>::operator[](int idx) noexcept{
    return *(&x + idx);
}

template <typename T>
const T& tvec2<T>::operator[](int idx) const noexcept{
    return *(&x + idx);
}

template <typename T>
tvec2<T>& tvec2<T>::operator+=(const self_t& rhs) noexcept
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}

template <typename T>
tvec2<T>& tvec2<T>::operator-=(const self_t& rhs) noexcept
{
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

template <typename T>
tvec2<T>& tvec2<T>::operator*=(const self_t& rhs) noexcept
{
    x *= rhs.x;
    y *= rhs.y;
    return *this;
}

template <typename T>
tvec2<T>& tvec2<T>::operator/=(const self_t& rhs) noexcept
{
    x /= rhs.x;
    y /= rhs.y;
    return *this;
}

template <typename T>
tvec2<T>& tvec2<T>::operator+=(T rhs) noexcept
{
    x += rhs;
    y += rhs;
    return *this;
}

template <typename T>
tvec2<T>& tvec2<T>::operator-=(T rhs) noexcept
{
    x -= rhs;
    y -= rhs;
    return *this;
}

template <typename T>
tvec2<T>& tvec2<T>::operator*=(T rhs) noexcept
{
    x *= rhs;
    y *= rhs;
    return *this;
}

template <typename T>
tvec2<T>& tvec2<T>::operator/=(T rhs) noexcept
{
    x /= rhs;
    y /= rhs;
    return *this;
}

template <typename T>
auto operator-(const tvec2<T>& vec) noexcept{
    return tvec2<T>(-vec.x,-vec.y);
}

template <typename T>
auto operator+(const tvec2<T>& lhs,const tvec2<T>& rhs) noexcept{
    return tvec2<T>(lhs.x+rhs.x, lhs.y+rhs.y);
}

template <typename T>
auto operator-(const tvec2<T>& lhs,const tvec2<T>& rhs) noexcept{
    return tvec2<T>(lhs.x-rhs.x, lhs.y-rhs.y);
}

template <typename T>
auto operator*(const tvec2<T>& lhs,const tvec2<T>& rhs) noexcept{
    return tvec2<T>(lhs.x*rhs.x, lhs.y*rhs.y);
}

template <typename T>
auto operator/(const tvec2<T>& lhs,const tvec2<T>& rhs) noexcept{
    return tvec2<T>(lhs.x/rhs.x, lhs.y/rhs.y);
}

template <typename T>
auto operator+(const tvec2<T>& lhs, T rhs) noexcept{
    return tvec2<T>(lhs.x+rhs, lhs.y+rhs);
}

template <typename T>
auto operator-(const tvec2<T>& lhs, T rhs) noexcept{
    return tvec2<T>(lhs.x-rhs, lhs.y-rhs);
}

template <typename T>
auto operator*(const tvec2<T>& lhs, T rhs) noexcept{
    return tvec2<T>(lhs.x*rhs, lhs.y*rhs);
}

template <typename T>
auto operator/(const tvec2<T>& lhs, T rhs) noexcept{
    return tvec2<T>(lhs.x/rhs, lhs.y/rhs);
}

template <typename T>
auto operator+(T lhs, const tvec2<T>& rhs) noexcept{
    return rhs + lhs;
}

template <typename T>
auto operator*(T lhs, const tvec2<T>& rhs) noexcept{
    return rhs * lhs;
}

template <typename T>
bool operator==(const tvec2<T>& lhs, const tvec2<T>& rhs) noexcept{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

template <typename T>
bool operator!=(const tvec2<T>& lhs, const tvec2<T>& rhs) noexcept{
    return !(lhs == rhs);
}

template <typename T>
bool operator<(const tvec2<T>& lhs,const tvec2<T>& rhs) noexcept{
    return std::tie(lhs.x,lhs.y) < std::tie(rhs.x,rhs.y);
}

template <typename T>
bool operator>(const tvec2<T>& lhs,const tvec2<T>& rhs) noexcept{
    return std::tie(lhs.x,lhs.y) > std::tie(rhs.x,rhs.y);
}

template <typename T>
auto dot(const tvec2<T>& lhs, const tvec2<T>& rhs) noexcept{
    return lhs.x*rhs.x + lhs.y*rhs.y;
}

template <typename T>
auto cross(const tvec2<T>& lhs, const tvec2<T>& rhs) noexcept{
    return lhs.x * rhs.y - lhs.y * rhs.x;
}

template <typename T>
auto cos(const tvec2<T>& lhs, const tvec2<T>& rhs) noexcept{
    return dot(lhs,rhs) / (lhs.length() * rhs.length());
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const tvec2<T>& vec){
    out << "("<<vec.x<<", "<<vec.y<<")";
    return out;
}

WZZ_MATH_END
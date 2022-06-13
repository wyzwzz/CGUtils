#pragma once
#include <cmath>
WZZ_MATH_BEGIN

template <typename T>
tvec3<T>::tvec3() noexcept
:tvec3(T(0))
{

}

template <typename T>
tvec3<T>::tvec3(T x, T y, T z) noexcept
:x(x),y(y),z(z)
{

}

template <typename T>
tvec3<T>::tvec3(T val) noexcept
:x(val),y(val),z(val)
{

}

template <typename T>
tvec3<T>::tvec3(uninitialized_t) noexcept{}

template <typename T>
tvec3<T>::tvec3(const self_t& v) noexcept
:x(v.x),y(v.y),z(v.z)
{

}

template <typename T>
tvec3<T>::tvec3(const tvec<T,3>& v) noexcept
:x(v[0]),y(v[1]),z(v[2])
{

}

template <typename T>
bool tvec3<T>::is_zero() const noexcept
{
    return x==T(0) && y==T(0) && z==T(0);
}

template <typename T>
bool tvec3<T>::is_finite() const noexcept
{
	return is_finite(x) && is_finite(y) && is_finite(z);
}

template <typename T>
auto tvec3<T>::length() const noexcept
{
    return std::sqrt(length_square());
}

template <typename T>
auto tvec3<T>::length_square() const noexcept
{
    return x*x + y*y + z*z;
}

template <typename T>
void tvec3<T>::normalize() noexcept{
    static_assert(std::is_floating_point_v<T>, "must be float point");
    this->operator/=(length());
}

template <typename T>
tvec3<T> tvec3<T>::normalized() const noexcept
{
    static_assert(std::is_floating_point_v<T>, "must be float point");
    return *this / length();
}

template <typename T>
template <typename F>
auto tvec3<T>::map(F&& f) const noexcept{
	using RT = remove_rcv_t<decltype(f(x))>;
	return tvec3<RT>(f(x),f(y),f(z));
}

template <typename T>
template <typename U>
auto tvec3<T>::convert_to() const noexcept{
	return tvec3<U>(U(x),U(y),U(z));
}

template <typename T>
T& tvec3<T>::operator[](int idx) noexcept{
    return *(&x + idx);
}

template <typename T>
const T& tvec3<T>::operator[](int idx) const noexcept{
    return *(&x + idx);
}

template <typename T>
tvec3<T>& tvec3<T>::operator+=(const self_t& rhs) noexcept
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

template <typename T>
tvec3<T>& tvec3<T>::operator-=(const self_t& rhs) noexcept
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

template <typename T>
tvec3<T>& tvec3<T>::operator*=(const self_t& rhs) noexcept
{
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
}

template <typename T>
tvec3<T>& tvec3<T>::operator/=(const self_t& rhs) noexcept
{
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    return *this;
}

template <typename T>
tvec3<T>& tvec3<T>::operator+=(T rhs) noexcept
{
    x += rhs;
    y += rhs;
    z += rhs;
    return *this;
}

template <typename T>
tvec3<T>& tvec3<T>::operator-=(T rhs) noexcept
{
    x -= rhs;
    y -= rhs;
    z -= rhs;
    return *this;
}

template <typename T>
tvec3<T>& tvec3<T>::operator*=(T rhs) noexcept
{
    x *= rhs;
    y *= rhs;
    z *= rhs;
    return *this;
}

template <typename T>
tvec3<T>& tvec3<T>::operator/=(T rhs) noexcept
{
    x /= rhs;
    y /= rhs;
    z /= rhs;
    return *this;
}

template <typename T>
auto operator-(const tvec3<T>& vec) noexcept{
    return tvec3<T>(-vec.x,-vec.y,-vec.z);
}

template <typename T>
auto operator+(const tvec3<T>& lhs,const tvec3<T>& rhs) noexcept{
    return tvec3<T>(lhs.x+rhs.x, lhs.y+rhs.y, lhs.z+rhs.z);
}

template <typename T>
auto operator-(const tvec3<T>& lhs,const tvec3<T>& rhs) noexcept{
    return tvec3<T>(lhs.x-rhs.x, lhs.y-rhs.y, lhs.z-rhs.z);
}

template <typename T>
auto operator*(const tvec3<T>& lhs,const tvec3<T>& rhs) noexcept{
    return tvec3<T>(lhs.x*rhs.x, lhs.y*rhs.y, lhs.z*rhs.z);
}

template <typename T>
auto operator/(const tvec3<T>& lhs,const tvec3<T>& rhs) noexcept{
    return tvec3<T>(lhs.x/rhs.x, lhs.y/rhs.y, lhs.z/rhs.z);
}

template <typename T>
auto operator+(const tvec3<T>& lhs, T rhs) noexcept{
    return tvec3<T>(lhs.x+rhs, lhs.y+rhs, lhs.z+rhs);
}

template <typename T>
auto operator-(const tvec3<T>& lhs, T rhs) noexcept{
    return tvec3<T>(lhs.x-rhs, lhs.y-rhs, lhs.z-rhs);
}

template <typename T>
auto operator*(const tvec3<T>& lhs, T rhs) noexcept{
    return tvec3<T>(lhs.x*rhs, lhs.y*rhs, lhs.z*rhs);
}

template <typename T>
auto operator/(const tvec3<T>& lhs, T rhs) noexcept{
    return tvec3<T>(lhs.x/rhs, lhs.y/rhs, lhs.z/rhs);
}

template <typename T>
auto operator+(T lhs, const tvec3<T>& rhs) noexcept{
    return rhs + lhs;
}

template <typename T>
auto operator*(T lhs, const tvec3<T>& rhs) noexcept{
    return rhs * lhs;
}

template <typename T>
bool operator==(const tvec3<T>& lhs, const tvec3<T>& rhs) noexcept{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

template <typename T>
bool operator!=(const tvec3<T>& lhs, const tvec3<T>& rhs) noexcept{
    return !(lhs == rhs);
}

template <typename T>
bool operator<(const tvec3<T>& lhs,const tvec3<T>& rhs) noexcept{
    return std::tie(lhs.x,lhs.y,lhs.z) < std::tie(rhs.x,rhs.y,rhs.z);
}

template <typename T>
bool operator>(const tvec3<T>& lhs,const tvec3<T>& rhs) noexcept{
    return std::tie(lhs.x,lhs.y,lhs.z) > std::tie(rhs.x,rhs.y,rhs.z);
}

template <typename T>
auto dot(const tvec3<T>& lhs, const tvec3<T>& rhs) noexcept{
    return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
}

template <typename T>
auto abs_dot(const tvec3<T>& lhs, const tvec3<T>& rhs) noexcept{
    return std::abs(lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z);
}

template <typename T>
auto cross(const tvec3<T>& lhs, const tvec3<T>& rhs) noexcept{
    return tvec3<T>(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x);
}

template <typename T>
auto cos(const tvec3<T>& lhs, const tvec3<T>& rhs) noexcept{
    return dot(lhs,rhs) / (lhs.length() * rhs.length());
}

template <typename T>
auto abs_cos(const tvec3<T>& lhs, const tvec3<T>& rhs) noexcept{
    return std::abs(dot(lhs,rhs) / (lhs.length() * rhs.length()));
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const tvec3<T>& vec){
    out << "("<<vec.x<<", "<<vec.y<<", "<<vec.z<<")";
    return out;
}

WZZ_MATH_END
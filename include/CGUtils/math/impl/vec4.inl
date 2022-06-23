#pragma once
#include <cmath>
WZZ_MATH_BEGIN

template <typename T>
tvec4<T>::tvec4() noexcept
    :tvec4(T(0))
{

}

template <typename T>
tvec4<T>::tvec4(T x, T y, T z, T w) noexcept
    :x(x),y(y),z(z),w(w)
{

}

template <typename T>
tvec4<T>::tvec4(T val) noexcept
    :x(val),y(val),z(val),w(val)
{

}

template <typename T>
tvec4<T>::tvec4(uninitialized_t) noexcept{}

template <typename T>
tvec4<T>::tvec4(const self_t& v) noexcept
    :x(v.x),y(v.y),z(v.z),w(v.w)
{

}

template <typename T>
tvec4<T>::tvec4(const tvec<T,4>& v) noexcept
    :x(v[0]),y(v[1]),z(v[2]),w(v[3])
{

}

template <typename T>
bool tvec4<T>::is_zero() const noexcept
{
    return x==T(0) && y==T(0) && z==T(0) && w==T(0);
}

template <typename T>
auto tvec4<T>::length() const noexcept
{
    return std::sqrt(length_square());
}

template <typename T>
auto tvec4<T>::length_square() const noexcept
{
    return x*x + y*y + z*z + w*w;
}

template <typename T>
void tvec4<T>::normalize() noexcept{
    static_assert(std::is_floating_point_v<T>, "must be float point");
    this->operator/=(length());
}

template <typename T>
tvec4<T> tvec4<T>::normalized() const noexcept
{
    static_assert(std::is_floating_point_v<T>, "must be float point");
    return *this / length();
}

template <typename T>
template <typename F>
auto tvec4<T>::map(F&& f) const noexcept{
	using RT = remove_rcv_t<decltype(f(x))>;
	return tvec4<RT>(f(x),f(y),f(z),f(w));
}

template <typename T>
template <typename U>
auto tvec4<T>::convert_to() const noexcept{
	return tvec3<U>(U(x),U(y),U(z),U(w));
}

template <typename T>
T& tvec4<T>::operator[](int idx) noexcept{
    return *(&x + idx);
}

template <typename T>
const T& tvec4<T>::operator[](int idx) const noexcept{
    return *(&x + idx);
}

template <typename T>
tvec4<T>& tvec4<T>::operator+=(const self_t& rhs) noexcept
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;
    return *this;
}

template <typename T>
tvec4<T>& tvec4<T>::operator-=(const self_t& rhs) noexcept
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    w -= rhs.w;
    return *this;
}

template <typename T>
tvec4<T>& tvec4<T>::operator*=(const self_t& rhs) noexcept
{
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    w *= rhs.w;
    return *this;
}

template <typename T>
tvec4<T>& tvec4<T>::operator/=(const self_t& rhs) noexcept
{
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    w /= rhs.w;
    return *this;
}

template <typename T>
tvec4<T>& tvec4<T>::operator+=(T rhs) noexcept
{
    x += rhs;
    y += rhs;
    z += rhs;
    w += rhs;
    return *this;
}

template <typename T>
tvec4<T>& tvec4<T>::operator-=(T rhs) noexcept
{
    x -= rhs;
    y -= rhs;
    z -= rhs;
    w -= rhs;
    return *this;
}

template <typename T>
tvec4<T>& tvec4<T>::operator*=(T rhs) noexcept
{
    x *= rhs;
    y *= rhs;
    z *= rhs;
    w *= rhs;
    return *this;
}

template <typename T>
tvec4<T>& tvec4<T>::operator/=(T rhs) noexcept
{
    x /= rhs;
    y /= rhs;
    z /= rhs;
    w /= rhs;
    return *this;
}

template <typename T>
tvec3<T> tvec4<T>::homogenize() const noexcept
{
    return tvec3(x/w,y/w,z/w);
}

template <typename T>
auto operator-(const tvec4<T>& vec) noexcept{
    return tvec4<T>(-vec.x,-vec.y,-vec.z,-vec.w);
}

template <typename T>
auto operator+(const tvec4<T>& lhs,const tvec4<T>& rhs) noexcept{
    return tvec4<T>(lhs.x+rhs.x, lhs.y+rhs.y, lhs.z+rhs.z, lhs.w+rhs.w);
}

template <typename T>
auto operator-(const tvec4<T>& lhs,const tvec4<T>& rhs) noexcept{
    return tvec4<T>(lhs.x-rhs.x, lhs.y-rhs.y, lhs.z-rhs.z, lhs.w-rhs.w);
}

template <typename T>
auto operator*(const tvec4<T>& lhs,const tvec4<T>& rhs) noexcept{
    return tvec4<T>(lhs.x*rhs.x, lhs.y*rhs.y, lhs.z*rhs.z, lhs.w*rhs.w);
}

template <typename T>
auto operator/(const tvec4<T>& lhs,const tvec4<T>& rhs) noexcept{
    return tvec4<T>(lhs.x/rhs.x, lhs.y/rhs.y, lhs.z/rhs.z, lhs.w/rhs.w);
}

template <typename T>
auto operator+(const tvec4<T>& lhs, T rhs) noexcept{
    return tvec3<T>(lhs.x+rhs, lhs.y+rhs, lhs.z+rhs, lhs.w+rhs);
}

template <typename T>
auto operator-(const tvec4<T>& lhs, T rhs) noexcept{
    return tvec4<T>(lhs.x-rhs, lhs.y-rhs, lhs.z-rhs, lhs.w-rhs);
}

template <typename T>
auto operator*(const tvec4<T>& lhs, T rhs) noexcept{
    return tvec4<T>(lhs.x*rhs, lhs.y*rhs, lhs.z*rhs, lhs.w*rhs);
}

template <typename T>
auto operator/(const tvec4<T>& lhs, T rhs) noexcept{
    return tvec4<T>(lhs.x/rhs, lhs.y/rhs, lhs.z/rhs, lhs.w/rhs);
}

template <typename T>
auto operator+(T lhs, const tvec4<T>& rhs) noexcept{
    return rhs + lhs;
}

template <typename T>
auto operator*(T lhs, const tvec4<T>& rhs) noexcept{
    return rhs * lhs;
}

template <typename T>
bool operator==(const tvec4<T>& lhs, const tvec4<T>& rhs) noexcept{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}

template <typename T>
bool operator!=(const tvec4<T>& lhs, const tvec4<T>& rhs) noexcept{
    return !(lhs == rhs);
}

template <typename T>
bool operator<(const tvec4<T>& lhs,const tvec4<T>& rhs) noexcept{
    return std::tie(lhs.x,lhs.y,lhs.z,lhs.w) < std::tie(rhs.x,rhs.y,rhs.z,rhs.w);
}

template <typename T>
bool operator>(const tvec4<T>& lhs,const tvec4<T>& rhs) noexcept{
    return std::tie(lhs.x,lhs.y,lhs.z,lhs.w) > std::tie(rhs.x,rhs.y,rhs.z,rhs.w);
}

template <typename T>
auto dot(const tvec4<T>& lhs, const tvec4<T>& rhs) noexcept{
    return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z + lhs.w*rhs.w;
}

template <typename T>
auto cos(const tvec4<T>& lhs, const tvec4<T>& rhs) noexcept{
    return dot(lhs,rhs) / (lhs.length() * rhs.length());
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const tvec4<T>& vec){
    out << "("<<vec.x<<", "<<vec.y<<", "<<vec.z<<", "<<vec.w<<")";
    return out;
}

WZZ_MATH_END
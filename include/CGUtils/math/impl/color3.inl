#pragma once

namespace wzz::math{

template <typename T>
tcolor3<T>::tcolor3() noexcept
:tcolor3(0)
{

}

template <typename T>
tcolor3<T>::tcolor3( T r, T g, T b ) noexcept
:r(r),g(g),b(b)
{

}

template <typename T>
tcolor3<T>::tcolor3( T v ) noexcept
:r(v),g(v),b(v)
{

}

template <typename T>
tcolor3<T>::tcolor3( uninitialized_t ) noexcept
{

}

template <typename T>
tcolor3<T>::tcolor3( const tvec3<T> &v ) noexcept
:r(v.x),g(v.y),b(v.z)
{

}

template <typename T>
tcolor3<T> tcolor3<T>::clamp( T min_v, T max_v ) const noexcept
{
	return tcolor3(
	  std::clamp(r,min_v,max_v),
	  std::clamp(g,min_v,max_v),
	  std::clamp(b,min_v,max_v)
	  );
}

template <typename T>
tcolor3<T> tcolor3<T>::saturate() const noexcept
{
	static_assert(std::is_floating_point_v<T>,"saturate should used for float");
	return clamp(0,1);
}

template <typename T>
bool tcolor3<T>::is_finite() const noexcept
{
	return std::isfinite(r) && std::isfinite(g) && std::isfinite(b);
}

template <typename T>
bool tcolor3<T>::is_valid() const noexcept
{
	return r >= 0 && g >= 0 && b >= 0;
}

template <typename T>
bool tcolor3<T>::is_black() const noexcept
{
	return r == 0 && g == 0 && b == 0;
}

template <typename T>
T tcolor3<T>::lum() const noexcept
{
	static_assert(std::is_floating_point_v<T>,"lum only used for float type color");
	return T(0.2126) * r + T(0.7152) * g + T(0.0722) * b;
}

template <typename T>
template <typename F>
auto tcolor3<T>::map( F &&f ) const noexcept
{
	using RT = remove_rcv_t<decltype(f(r))>;
	return tcolor3<RT>(f(r),f(g),f(b));
}

template <typename T>
T &tcolor3<T>::operator[]( int idx ) noexcept
{
	return *(&r + idx);
}

template <typename T>
const T &tcolor3<T>::operator[]( int idx ) const noexcept
{
	return *(&r + idx);
}

template <typename T>
tcolor3<T> &tcolor3<T>::operator+=( const tcolor3 &rhs ) noexcept
{
	r += rhs.r;
	g += rhs.g;
	b += rhs.b;
	return *this;
}

template <typename T>
tcolor3<T> &tcolor3<T>::operator-=( const tcolor3 &rhs ) noexcept
{
	r -= rhs.r;
	g -= rhs.g;
	b -= rhs.b;
	return *this;
}

template <typename T>
tcolor3<T> &tcolor3<T>::operator*=( const tcolor3 &rhs ) noexcept
{
	r *= rhs.r;
	g *= rhs.g;
	b *= rhs.b;
	return *this;
}

template <typename T>
tcolor3<T> &tcolor3<T>::operator/=( const tcolor3 &rhs ) noexcept
{
	r /= rhs.r;
	g /= rhs.g;
	b /= rhs.b;
	return *this;
}

template <typename T>
tcolor3<T> &tcolor3<T>::operator+=( T rhs ) noexcept
{
	r += rhs;
	g += rhs;
	b += rhs;
	return *this;
}

template <typename T>
tcolor3<T> &tcolor3<T>::operator-=( T rhs ) noexcept
{
	r -= rhs;
	g -= rhs;
	b -= rhs;
	return *this;
}

template <typename T>
tcolor3<T> &tcolor3<T>::operator*=( T rhs ) noexcept
{
	r *= rhs;
	g *= rhs;
	b *= rhs;
	return *this;
}

template <typename T>
tcolor3<T> &tcolor3<T>::operator/=( T rhs ) noexcept
{
	r /= rhs;
	g /= rhs;
	b /= rhs;
	return *this;
}

//---------------
template <typename T>
tcolor3<T> operator-(const tcolor3<T>& color) noexcept{
	return tcolor3<T>(-color.r,-color.g,-color.b);
}

template <typename T>
tcolor3<T> operator+(const tcolor3<T>& lhs,const tcolor3<T>& rhs) noexcept{
	return tcolor3<T>(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b);
}

template <typename T>
tcolor3<T> operator-(const tcolor3<T>& lhs,const tcolor3<T>& rhs) noexcept{
	return tcolor3<T>(lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b);
}

template <typename T>
tcolor3<T> operator*(const tcolor3<T>& lhs,const tcolor3<T>& rhs) noexcept{
	return tcolor3<T>(lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b);
}

template <typename T>
tcolor3<T> operator/(const tcolor3<T>& lhs,const tcolor3<T>& rhs) noexcept{
	return tcolor3<T>(lhs.r / rhs.r, lhs.g / rhs.g, lhs.b / rhs.b);
}

template <typename T>
tcolor3<T> operator+(const tcolor3<T>& lhs,T rhs) noexcept{
	return tcolor3<T>(lhs.r + rhs, lhs.g + rhs, lhs.b + rhs);
}

template <typename T>
tcolor3<T> operator-(const tcolor3<T>& lhs,T rhs) noexcept{
	return tcolor3<T>(lhs.r - rhs, lhs.g - rhs, lhs.b - rhs);
}

template <typename T>
tcolor3<T> operator*(const tcolor3<T>& lhs,T rhs) noexcept{
	return tcolor3<T>(lhs.r * rhs, lhs.g * rhs, lhs.b * rhs);
}

template <typename T>
tcolor3<T> operator/(const tcolor3<T>& lhs,T rhs) noexcept{
	return tcolor3<T>(lhs.r / rhs, lhs.g / rhs, lhs.b / rhs);
}

template<typename T>
tcolor3<T> operator+(T lhs, const tcolor3<T> &rhs) noexcept{
	return rhs + lhs;
}

template<typename T>
tcolor3<T> operator-(T lhs, const tcolor3<T> &rhs) noexcept{
	return tcolor3<T>(lhs - rhs.r, lhs - rhs.g, lhs - rhs.b);
}

template<typename T>
tcolor3<T> operator*(T lhs, const tcolor3<T> &rhs) noexcept{
	return rhs * lhs;
}

template<typename T>
bool operator==(const tcolor3<T> &lhs, const tcolor3<T> &rhs) noexcept{
	return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}

template<typename T>
bool operator!=(const tcolor3<T> &lhs, const tcolor3<T> &rhs) noexcept{
	return !(lhs == rhs);
}

template <typename T,typename>
color3b to_color3b(const tcolor3<T>& color) noexcept{
	return color.saturate().map([](T v){
		return static_cast<unsigned char>(v * 255);
	});
}

template<typename T,typename>
tcolor3<T> from_color3b(const color3b& color) noexcept{
	return color.map([](T v){
		return static_cast<T>(v / T(255));
	});
}

}
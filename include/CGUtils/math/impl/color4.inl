#pragma once

namespace wzz::math{

template <typename T>
tcolor4<T>::tcolor4() noexcept
:tcolor4(0)
{

}

template <typename T>
tcolor4<T>::tcolor4( T r, T g, T b, T a ) noexcept
:r(r),g(g),b(b),a(a)
{

}

template <typename T>
tcolor4<T>::tcolor4( T v ) noexcept
:r(v),g(v),b(v),a(v)
{

}

template <typename T>
tcolor4<T>::tcolor4( uninitialized_t ) noexcept
{

}

template <typename T>
tcolor4<T>::tcolor4( const tvec4<T> &v ) noexcept
:r(v.x),g(v.y),b(v.z),a(v.w)
{

}

template <typename T>
tcolor4<T> tcolor4<T>::clamp( T min_v, T max_v ) const noexcept
{
	return tcolor4(
	  std::clamp(r,min_v,max_v),
	  std::clamp(g,min_v,max_v),
	  std::clamp(b,min_v,max_v),
	  std::clamp(a,min_v,max_v)
	  );
}

template <typename T>
tcolor4<T> tcolor4<T>::saturate() const noexcept
{
	static_assert(std::is_floating_point_v<T>,"saturate should used for float");
	return clamp(0,1);
}

template <typename T>
bool tcolor4<T>::is_finite() const noexcept
{
	return std::isfinite(r) && std::isfinite(g) && std::isfinite(b) && std::isfinite(a);
}

template <typename T>
bool tcolor4<T>::is_valid() const noexcept
{
	return r >= 0 && g >= 0 && b >= 0 && a >= 0;
}

template <typename T>
bool tcolor4<T>::is_black() const noexcept
{
	return r == 0 && g == 0 && b == 0 && a == 0;
}

template <typename T>
template <typename F>
auto tcolor4<T>::map( F &&f ) const noexcept
{
	using RT = remove_rcv_t<decltype(f(r))>;
	return tcolor4<RT>(f(r),f(g),f(b),f(a));
}

template <typename T>
T &tcolor4<T>::operator[]( int idx ) noexcept
{
	return *(&r + idx);
}

template <typename T>
const T &tcolor4<T>::operator[]( int idx ) const noexcept
{
	return *(&r + idx);
}

template <typename T>
tcolor4<T> &tcolor4<T>::operator+=( const tcolor4 &rhs ) noexcept
{
	r += rhs.r;
	g += rhs.g;
	b += rhs.b;
	a += rhs.a;
	return *this;
}

template <typename T>
tcolor4<T> &tcolor4<T>::operator-=( const tcolor4 &rhs ) noexcept
{
	r -= rhs.r;
	g -= rhs.g;
	b -= rhs.b;
	a -= rhs.a;
	return *this;
}

template <typename T>
tcolor4<T> &tcolor4<T>::operator*=( const tcolor4 &rhs ) noexcept
{
	r *= rhs.r;
	g *= rhs.g;
	b *= rhs.b;
	a *= rhs.a;
	return *this;
}

template <typename T>
tcolor4<T> &tcolor4<T>::operator/=( const tcolor4 &rhs ) noexcept
{
	r /= rhs.r;
	g /= rhs.g;
	b /= rhs.b;
	a /= rhs.a;
	return *this;
}

template <typename T>
tcolor4<T> &tcolor4<T>::operator+=( T rhs ) noexcept
{
	r += rhs;
	g += rhs;
	b += rhs;
	a += rhs;
	return *this;
}
template <typename T>
tcolor4<T> &tcolor4<T>::operator-=( T rhs ) noexcept
{
	r -= rhs;
	g -= rhs;
	b -= rhs;
	a -= rhs;
	return *this;
}
template <typename T>
tcolor4<T> &tcolor4<T>::operator*=( T rhs ) noexcept
{
	r *= rhs;
	g *= rhs;
	b *= rhs;
	a *= rhs;
	return *this;
}
template <typename T>
tcolor4<T> &tcolor4<T>::operator/=( T rhs ) noexcept
{
	r /= rhs;
	g /= rhs;
	b /= rhs;
	a /= rhs;
	return *this;
}

template <typename T>
tcolor4<T> operator-(const tcolor4<T>& color) noexcept{
	return tcolor4<T>(-color.r,-color.g,-color.b,-color.a);
}

template <typename T>
tcolor4<T> operator+(const tcolor4<T>& lhs,const tcolor4<T>& rhs) noexcept{
	return tcolor4<T>(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b, lhs.a + rhs.a);
}

template <typename T>
tcolor4<T> operator-(const tcolor4<T>& lhs,const tcolor4<T>& rhs) noexcept{
	return tcolor4<T>(lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b, lhs.a - rhs.a);
}

template <typename T>
tcolor4<T> operator*(const tcolor4<T>& lhs,const tcolor4<T>& rhs) noexcept{
	return tcolor4<T>(lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b, lhs.a * rhs.a);
}

template <typename T>
tcolor4<T> operator/(const tcolor4<T>& lhs,const tcolor4<T>& rhs) noexcept{
	return tcolor4<T>(lhs.r / rhs.r, lhs.g / rhs.g, lhs.b / rhs.b, lhs.a / rhs.a);
}

template <typename T>
tcolor4<T> operator+(const tcolor4<T>& lhs,T rhs) noexcept{
	return tcolor4<T>(lhs.r + rhs, lhs.g + rhs, lhs.b + rhs, lhs.a + rhs);
}

template <typename T>
tcolor4<T> operator-(const tcolor4<T>& lhs,T rhs) noexcept{
	return tcolor4<T>(lhs.r - rhs, lhs.g - rhs, lhs.b - rhs, lhs.a - rhs);
}

template <typename T>
tcolor4<T> operator*(const tcolor4<T>& lhs,T rhs) noexcept{
	return tcolor4<T>(lhs.r * rhs, lhs.g * rhs, lhs.b * rhs, lhs.a * rhs);
}

template <typename T>
tcolor4<T> operator/(const tcolor4<T>& lhs,T rhs) noexcept{
	return tcolor4<T>(lhs.r / rhs, lhs.g / rhs, lhs.b / rhs, lhs.a / rhs);
}

template<typename T>
tcolor4<T> operator+(T lhs, const tcolor4<T> &rhs) noexcept{
	return rhs + lhs;
}

template<typename T>
tcolor4<T> operator-(T lhs, const tcolor4<T> &rhs) noexcept{
	return -(rhs - lhs);
}

template<typename T>
tcolor4<T> operator*(T lhs, const tcolor4<T> &rhs) noexcept{
	return rhs * lhs;
}

template<typename T>
bool operator==(const tcolor4<T> &lhs, const tcolor4<T> &rhs) noexcept{
	return (lhs - rhs).is_black();
}

template<typename T>
bool operator!=(const tcolor4<T> &lhs, const tcolor4<T> &rhs) noexcept{
	return !(lhs == rhs);
}

template <typename T,typename>
color4b to_color4b(const tcolor4<T>& color) noexcept{
	return color.saturate().map([](T v){
		return static_cast<unsigned char>(v * 255);
	});
}

template<typename T,typename >
tcolor4<T> from_color4b(const color4b& color) noexcept{
	return color.map([](T v){
		return static_cast<T>(v / T(255));
	});
}

}
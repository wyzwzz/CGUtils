#pragma once

namespace wzz::math{

template <typename T>
tcolor2<T>::tcolor2() noexcept
:tcolor2(0)
{

}

template <typename T>
tcolor2<T>::tcolor2( T r, T g ) noexcept
:r(r),g(g)
{

}

template <typename T>
tcolor2<T>::tcolor2( T v ) noexcept
:r(v),g(v)
{

}

template <typename T>
tcolor2<T>::tcolor2( uninitialized_t ) noexcept
{

}

template <typename T>
tcolor2<T>::tcolor2( const tvec2<T> &v ) noexcept
:r(v.x),g(v.y)
{

}

template <typename T>
tcolor2<T> tcolor2<T>::clamp( T min_v, T max_v ) const noexcept
{
	return tcolor2(std::clamp(r,min_v,max_v),
					std::clamp(g,min_v,max_v));
}

template <typename T>
tcolor2<T> tcolor2<T>::saturate() const noexcept
{
	static_assert(std::is_floating_point_v<T>,"saturate should used for float");
	return clamp(0,1);
}

template <typename T>
bool tcolor2<T>::is_finite() const noexcept
{
	return std::isfinite(r) && std::isfinite(g);
}

template <typename T>
bool tcolor2<T>::is_valid() const noexcept
{
	return r >= 0 && g >= 0;
}

template <typename T>
bool tcolor2<T>::is_black() const noexcept
{
	return r == 0 && g == 0;
}

template <typename T>
template <typename F>
auto tcolor2<T>::map( F &&f ) const noexcept
{
	using RT = remove_rcv_t<decltype(F(r))>;
	return tcolor2<RT>(f(r),f(g));
}

template <typename T>
T &tcolor2<T>::operator[]( int idx ) noexcept
{
	return *(&r + idx);
}

template <typename T>
const T &tcolor2<T>::operator[]( int idx ) const noexcept
{
	return *(&r + idx);
}

template <typename T>
tcolor2<T> &tcolor2<T>::operator+=( const tcolor2 &rhs ) noexcept
{
	r += rhs.r;
	g += rhs.g;
	return *this;
}

template <typename T>
tcolor2<T> &tcolor2<T>::operator-=( const tcolor2 &rhs ) noexcept
{
	r -= rhs.r;
	g -= rhs.g;
	return *this;
}

template <typename T>
tcolor2<T> &tcolor2<T>::operator*=( const tcolor2 &rhs ) noexcept
{
	r *= rhs.r;
	g *= rhs.g;
	return *this;
}

template <typename T>
tcolor2<T> &tcolor2<T>::operator/=( const tcolor2 &rhs ) noexcept
{
	r /= rhs.r;
	g /= rhs.g;
	return *this;
}

template <typename T>
tcolor2<T> &tcolor2<T>::operator+=( T rhs ) noexcept
{
	r += rhs;
	g += rhs;
	return *this;
}
template <typename T>
tcolor2<T> &tcolor2<T>::operator-=( T rhs ) noexcept
{
	r -= rhs;
	g -= rhs;
	return *this;
}
template <typename T>
tcolor2<T> &tcolor2<T>::operator*=( T rhs ) noexcept
{
	r *= rhs;
	g *= rhs;
	return *this;
}
template <typename T>
tcolor2<T> &tcolor2<T>::operator/=( T rhs ) noexcept
{
	r /= rhs;
	g /= rhs;
	return *this;
}

//---------

template <typename T>
tcolor2<T> operator-(const tcolor2<T>& color) noexcept{
	return tcolor2<T>(-color.r,-color.g);
}

template <typename T>
tcolor2<T> operator+(const tcolor2<T>& lhs,const tcolor2<T>& rhs) noexcept{
	return tcolor2<T>(lhs.r+rhs.r,lhs.g+lhs.g);
}

template <typename T>
tcolor2<T> operator-(const tcolor2<T>& lhs,const tcolor2<T>& rhs) noexcept{
	return tcolor2<T>(lhs.r-rhs.r,lhs.g-lhs.g);
}

template <typename T>
tcolor2<T> operator*(const tcolor2<T>& lhs,const tcolor2<T>& rhs) noexcept{
	return tcolor2<T>(lhs.r*rhs.r,lhs.g*lhs.g);
}

template <typename T>
tcolor2<T> operator/(const tcolor2<T>& lhs,const tcolor2<T>& rhs) noexcept{
	return tcolor2<T>(lhs.r/rhs.r,lhs.g/lhs.g);
}

template <typename T>
tcolor2<T> operator+(const tcolor2<T>& lhs,T rhs) noexcept{
	return tcolor2<T>(lhs.r+rhs,lhs.g+rhs);
}

template <typename T>
tcolor2<T> operator-(const tcolor2<T>& lhs,T rhs) noexcept{
	return tcolor2<T>(lhs.r-rhs,lhs.g-lhs);
}

template <typename T>
tcolor2<T> operator*(const tcolor2<T>& lhs,T rhs) noexcept{
	return tcolor2<T>(lhs.r * rhs,lhs.g * lhs);
}

template <typename T>
tcolor2<T> operator/(const tcolor2<T>& lhs,T rhs) noexcept{
	return tcolor2<T>(lhs.r / rhs,lhs.g / lhs);
}

template<typename T>
tcolor2<T> operator+(T lhs, const tcolor2<T> &rhs) noexcept{
	return rhs + lhs;
}

template<typename T>
tcolor2<T> operator-(T lhs, const tcolor2<T> &rhs) noexcept{
	return tcolor2<T>(lhs - rhs.r, lhs - rhs.g);
}

template<typename T>
tcolor2<T> operator*(T lhs, const tcolor2<T> &rhs) noexcept{
	return rhs * lhs;
}

template<typename T>
bool operator==(const tcolor2<T> &lhs, const tcolor2<T> &rhs) noexcept{
	return lhs.r == rhs. r && lhs.g == rhs.g;
}

template<typename T>
bool operator!=(const tcolor2<T> &lhs, const tcolor2<T> &rhs) noexcept{
	return !(lhs == rhs);
}

template <typename T,typename >
color2b to_color2b(const tcolor2<T>& color) noexcept{
	return color.saturate().map([](T s){
		return static_cast<unsigned char>(s * 255);
	});
}

template<typename T,typename >
tcolor2<T> from_color2b(const color2b& color) noexcept{
	return color.map([](unsigned char s){
		return static_cast<T>(s / T(255));
	});
}

}
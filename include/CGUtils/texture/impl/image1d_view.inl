#pragma once

namespace wzz::texture{

template <typename T, bool CONST>
image1d_view_t<T, CONST>::image1d_view_t(data_t data )
:data(data)
{

}
template <typename T, bool CONST>
void image1d_view_t<T, CONST>::swap(image1d_view_t& other) noexcept
{
	data.swap(other.data);
}
template <typename T, bool CONST>
int image1d_view_t<T, CONST>::width() const noexcept
{
	return data.shape()[0];
}
template <typename T, bool CONST>
int image1d_view_t<T, CONST>::size() const noexcept
{
	return data.shape()[0];
}
template <typename T, bool CONST>
T &image1d_view_t<T, CONST>::operator()( int x ) noexcept
{
	return data[x];
}
template <typename T, bool CONST>
const T &image1d_view_t<T, CONST>::operator()( int x ) const noexcept
{
	return data[x];
}
template <typename T, bool CONST>
T &image1d_view_t<T, CONST>::at( int x ) noexcept
{
	return data[x];
}
template <typename T, bool CONST>
const T &image1d_view_t<T, CONST>::at( int x ) const noexcept
{
	return data[x];
}
template <typename T, bool CONST>
typename image1d_view_t<T,CONST>::data_t &image1d_view_t<T, CONST>::get_data() noexcept
{
	return data;
}
template <typename T, bool CONST>
const typename image1d_view_t<T,CONST>::data_t &image1d_view_t<T, CONST>::get_data() const noexcept
{
	return data;
}
template <typename T, bool CONST>
image1d_view_t<T,CONST> image1d_view_t<T, CONST>::sub_view( int beg_x, int end_x )
{
	return image1d_view_t(data.template get_subview<T,CONST>(math::tvec<int,1>{beg_x},math::tvec<int,1>{end_x - beg_x}));
}
template <typename T, bool CONST>
image1d_view_t<T, true> image1d_view_t<T, CONST>::sub_view( int beg_x, int end_x ) const
{
	return image1d_view_t(data.template get_subview<T,true>(math::tvec<int,1>{beg_x},math::tvec<int,1>{end_x - beg_x}));
}
template <typename T, bool CONST>
image1d_view_t<T, true> image1d_view_t<T, CONST>::sub_view_const( int beg_x, int end_x ) const
{
	return image1d_view_t(data.template get_subview<T,true>(math::tvec<int,1>{beg_x},math::tvec<int,1>{end_x - beg_x}));
}

}
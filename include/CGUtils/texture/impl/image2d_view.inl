#pragma once

namespace wzz::texture{

template <typename T, bool CONST>
image2d_view_t<T, CONST>::image2d_view_t( data_t data )
:data(data)
{

}

template <typename T, bool CONST>
void image2d_view_t<T, CONST>::swap( image2d_view_t &other )
{
	data.swap(other.data);
}

template <typename T, bool CONST>
int image2d_view_t<T, CONST>::width() const noexcept
{
	return data.shape()[0];
}

template <typename T, bool CONST>
int image2d_view_t<T, CONST>::height() const noexcept
{
	return data.shape()[1];
}

template <typename T, bool CONST>
math::vec2i image2d_view_t<T, CONST>::size() const noexcept
{
	return {data.shape()[0],data.shape()[1]};
}

template <typename T, bool CONST>
T &image2d_view_t<T, CONST>::operator()( int x, int y ) noexcept
{
	return data(x,y);
}

template <typename T, bool CONST>
const T &image2d_view_t<T, CONST>::operator()( int x, int y ) const noexcept
{
	return data(x,y);
}

template <typename T, bool CONST>
T &image2d_view_t<T, CONST>::at( int x, int y ) noexcept
{
	return data.at(x,y);
}

template <typename T, bool CONST>
const T &image2d_view_t<T, CONST>::at( int x, int y ) const noexcept
{
	return data.at(x,y);
}

template <typename T, bool CONST>
typename image2d_view_t<T,CONST>::data_t &image2d_view_t<T, CONST>::get_data() noexcept
{
	return data;
}

template <typename T, bool CONST>
const typename image2d_view_t<T,CONST>::data_t &image2d_view_t<T, CONST>::get_data() const noexcept
{
	return data;
}

template <typename T, bool CONST>
image2d_view_t<T,CONST> image2d_view_t<T, CONST>::sub_view( int beg_x, int beg_y, int end_x, int end_y )
{
	return image2d_view_t<T,CONST>({beg_x,beg_y},{end_x-beg_x,end_y-beg_y});
}

template <typename T, bool CONST>
image2d_view_t<T, true> image2d_view_t<T, CONST>::sub_view( int beg_x, int beg_y, int end_x, int end_y ) const
{
	return image2d_view_t<T,true>({beg_x,beg_y},{end_x-beg_x,end_y-beg_y});
}

template <typename T, bool CONST>
image2d_view_t<T, true> image2d_view_t<T, CONST>::sub_view_const( int beg_x, int beg_y, int end_x, int end_y ) const
{
	return image2d_view_t<T,true>({beg_x,beg_y},{end_x-beg_x,end_y-beg_y});
}
}
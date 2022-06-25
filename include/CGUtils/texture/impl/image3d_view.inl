#pragma once

namespace wzz::texture{

template <typename T, bool CONST>
image3d_view_t<T, CONST>::image3d_view_t( image3d_view_t::data_t data )
:data(data)
{

}

template <typename T, bool CONST>
void image3d_view_t<T, CONST>::swap( image3d_view_t &other )
{
	data.swap(other.data);
}

template <typename T, bool CONST>
int image3d_view_t<T, CONST>::width() const noexcept
{
	return data.shape()[0];
}

template <typename T, bool CONST>
int image3d_view_t<T, CONST>::height() const noexcept
{
	return data.shape()[1];
}

template <typename T, bool CONST>
int image3d_view_t<T, CONST>::depth() const noexcept
{
	return data.shape()[2];
}

template <typename T, bool CONST>
math::vec3i image3d_view_t<T, CONST>::size() const noexcept
{
	return {data.shape()[0],data.shape()[1],data.shape()[2]};
}

template <typename T, bool CONST>
T &image3d_view_t<T, CONST>::operator()( int x, int y, int z ) noexcept
{
	return data(x,y,z);
}

template <typename T, bool CONST>
const T &image3d_view_t<T, CONST>::operator()( int x, int y, int z ) const noexcept
{
	return data(x,y,z);
}

template <typename T, bool CONST>
T &image3d_view_t<T, CONST>::at( int x, int y, int z ) noexcept
{
	return data.at(x,y,z);
}

template <typename T, bool CONST>
const T &image3d_view_t<T, CONST>::at( int x, int y, int z ) const noexcept
{
	return data.at(x,y,z);
}

template <typename T, bool CONST>
typename image3d_view_t<T,CONST>::data_t &image3d_view_t<T, CONST>::get_data() noexcept
{
	return data;
}

template <typename T, bool CONST>
const typename image3d_view_t<T,CONST>::data_t &image3d_view_t<T, CONST>::get_data() const noexcept
{
	return data;
}

template <typename T, bool CONST>
image3d_view_t<T,CONST> image3d_view_t<T, CONST>::sub_view( int beg_x, int beg_y, int beg_z, int end_x, int end_y, int end_z )
{
	return image3d_view_t<T,CONST>({beg_x,beg_y,beg_z},
									 {end_x - beg_x,end_y - beg_y,end_z - beg_z});
}

template <typename T, bool CONST>
image3d_view_t<T, true> image3d_view_t<T, CONST>::sub_view( int beg_x, int beg_y, int beg_z, int end_x, int end_y, int end_z ) const
{
	return image3d_view_t<T,true>({beg_x,beg_y,beg_z},
									 {end_x - beg_x,end_y - beg_y,end_z - beg_z});
}

template <typename T, bool CONST>
image3d_view_t<T, true> image3d_view_t<T, CONST>::sub_view_const( int beg_x, int beg_y, int beg_z, int end_x, int end_y, int end_z ) const
{
	return image3d_view_t<T,true>({beg_x,beg_y,beg_z},
									 {end_x - beg_x,end_y - beg_y,end_z - beg_z});
}

}
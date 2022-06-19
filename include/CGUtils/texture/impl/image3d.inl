#pragma once

namespace wzz::texture{

template <typename T>
image3d_t<T>::image3d_t( int w, int h, int d, uninitialized_t )
:data({w,h,d},uninitialized_t{})
{

}

template <typename T>
image3d_t<T>::image3d_t( int w, int h, int d, const T *src )
:data(data_t::from_linear_array({w,h,d},src))
{

}

template <typename T>
image3d_t<T>::image3d_t( int w, int h, int d, const T &init_val )
:data({w,h,d},init_val)
{

}

template <typename T>
image3d_t<T>::image3d_t( const data_t &other )
:data(other)
{

}

template <typename T>
image3d_t<T>::image3d_t( data_t &&other ) noexcept
:data(std::move(other))
{

}

template <typename T>
image3d_t<T>::image3d_t( image3d_t &&other ) noexcept
:data(std::move(other.data))
{

}

template <typename T>
image3d_t<T> &image3d_t<T>::operator=( image3d_t &&other ) noexcept
{
	data = std::move(other.data);
	return *this;
}

template <typename T>
void image3d_t<T>::initialize( int w, int h, int d, uninitialized_t )
{
	*this = image3d_t<T>({w,h,d},uninitialized_t{});
}

template <typename T>
void image3d_t<T>::initialize( int w, int h, int d, const T *src )
{
	*this = image3d_t<T>({w,h,d},src);
}

template <typename T>
void image3d_t<T>::initialize( int w, int h, int d, const T &init_val )
{
	*this = image3d_t<T>({w,h,d},init_val);
}

template <typename T>
void image3d_t<T>::swap( image3d_t &other )
{
	data.swap(other.data);
}

template <typename T>
void image3d_t<T>::destroy()
{
	data.destroy();
}

template <typename T>
bool image3d_t<T>::is_available() const noexcept
{
	return data.is_available();
}

template <typename T>
int image3d_t<T>::width() const noexcept
{
	return data.shape()[0];
}

template <typename T>
int image3d_t<T>::height() const noexcept
{
	return data.shape()[1];
}

template <typename T>
int image3d_t<T>::depth() const noexcept
{
	return data.shape()[2];
}

template <typename T>
math::vec3i image3d_t<T>::size() const noexcept
{
	return {data.shape()[0],data.shape()[1],data.shape()[2]};
}

template <typename T>
void image3d_t<T>::clear( const T &value )
{
	data.map_inplace([&](T& v){
		v =value;
	});
}

template <typename T>
T &image3d_t<T>::operator()( int x, int y, int z ) noexcept
{
	return data(x,y,z);
}

template <typename T>
const T &image3d_t<T>::operator()( int x, int y, int z ) const noexcept
{
	return data(x,y,z);
}

template <typename T>
T &image3d_t<T>::at( int x, int y, int z ) noexcept
{
	return data.at({x,y,z});
}

template <typename T>
const T &image3d_t<T>::at( int x, int y, int z ) const noexcept
{
	return data.at({x,y,z});
}

template <typename T>
typename image3d_t<T>::data_t &image3d_t<T>::get_data() noexcept
{
	return data;
}

template <typename T>
const typename image3d_t<T>::data_t &image3d_t<T>::get_data() const noexcept
{
	return data;
}

template <typename T>
T *image3d_t<T>::get_raw_data() noexcept
{
	return data.raw_data();
}

template <typename T>
const T *image3d_t<T>::get_raw_data() const noexcept
{
	return data.raw_data();
}

template <typename T>
template <typename F>
auto image3d_t<T>::map( F &&f ) const
{
	using RT = remove_rcv_t<decltype(f(std::declval<T>()))>;
	return image3d_t<RT>(data.map(std::forward<F>(f)));
}

template <typename T>
template <typename F>
void image3d_t<T>::map_inplace( F &&f ) const
{
	return data.map_inplace(std::forward<F>(f));
}

template <typename T>
image3d_t<T> image3d_t<T>::sub_image( int beg_x, int beg_y, int beg_z, int end_x, int end_y, int end_z ) const
{
	assert(is_available());
	assert(beg_x >= 0 && beg_x < end_x && end_x <= width());
	assert(beg_y >= 0 && beg_y < end_y && end_y <= height());
	assert(beg_z >= 0 && beg_z < end_z && end_z <= depth());
	int x_size = end_x - beg_x;
	int y_size = end_y - beg_y;
	int z_size = end_z - beg_z;
	image3d_t<T> ret(x_size,y_size,z_size);
	for(int z = 0; z < z_size; ++z)
		for(int y = 0; y < y_size; ++y)
			for(int x = 0; x < x_size; ++x)
				ret(x,y,z) = at(beg_x + x,beg_y + y,beg_z + z);
	return ret;

}

template <typename T>
image3d_view_t<T, false> image3d_t<T>::sub_view( int beg_x, int beg_y, int beg_z, int end_x, int end_y, int end_z )
{
	return image3d_view_t<T,false>(data.get_subview({beg_x,beg_y,beg_z},
									 {end_x - beg_x,
									 end_y - beg_y,
									 end_z - beg_z}));
}

template <typename T>
image3d_view_t<T, true> image3d_t<T>::sub_view( int beg_x, int beg_y, int beg_z, int end_x, int end_y, int end_z ) const
{
	return image3d_view_t<T,true>(data.get_subview({beg_x,beg_y,beg_z},
									 {end_x - beg_x,
									   end_y - beg_y,
									   end_z - beg_z}));
}

template <typename T>
image3d_view_t<T, true> image3d_t<T>::sub_view_const( int beg_x, int beg_y, int beg_z, int end_x, int end_y, int end_z ) const
{
	return image3d_view_t<T,true>(data.get_const_subview({beg_x,beg_y,beg_z},
									{end_x - beg_x,
									  end_y - beg_y,
									  end_z - beg_z}));
}

template <typename T>
template <typename U>
auto image3d_t<T>::operator+( const image3d_t<U> &other ) const
{
	assert(is_available() && other.is_available());
	assert(size() == other.size());

	using RT = remove_rcv_t<decltype(std::declval<T>() + std::declval<U>())>;
	image2d_t<RT> ret(width(),height(),depth());
	for(int z = 0; z < depth(); ++z)
		for(int y = 0; y < height(); ++y){
			for(int x = 0; x < width(); ++x){
				ret(x,y,z) = operator()(x,y,z) + other(x,y,z);
			}
		}
	return ret;
}

template <typename T>
template <typename U>
auto image3d_t<T>::operator*( const image3d_t<U> &other ) const
{
	assert(is_available() && other.is_available());
	assert(size() == other.size());

	using RT = remove_rcv_t<decltype(std::declval<T>() * std::declval<U>())>;
	image2d_t<RT> ret(width(),height(),depth());
	for(int z = 0; z < depth(); ++z)
		for(int y = 0; y < height(); ++y){
			for(int x = 0; x < width(); ++x){
				ret(x,y,z) = operator()(x,y,z) * other(x,y,z);
			}
		}
	return ret;
}

template <typename T>
template <typename U>
image3d_t<T> &image3d_t<T>::operator+=( const image3d_t<U> &other ) const
{
	assert(is_available() && other.is_available());
	assert(size() == other.size());

	for(int z = 0; z < depth(); ++z)
		for(int y = 0; y < height(); ++y){
			for(int x = 0; x < width(); ++x){
				at(x,y,z) += other(x,y,z);
			}
		}
	return *this;
}

template <typename T>
template <typename U>
image3d_t<T> &image3d_t<T>::operator*=( const image3d_t<U> &other ) const
{
	assert(is_available() && other.is_available());
	assert(size() == other.size());

	for(int z = 0; z < depth(); ++z)
		for(int y = 0; y < height(); ++y){
			for(int x = 0; x < width(); ++x){
				at(x,y,z) *= other(x,y,z);
			}
		}
	return *this;
}

}
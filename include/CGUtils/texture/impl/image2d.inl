#pragma once

namespace wzz::texture{

template <typename T>
image2d_t<T>::image2d_t( int w, int h, uninitialized_t )
:data({w,h},uninitialized_t{})
{

}

template <typename T>
image2d_t<T>::image2d_t( int w, int h, const T *src )
:data(data_t::from_linear_array({w,h},src))
{

}

template <typename T>
image2d_t<T>::image2d_t( int w, int h, const T &init_val )
:data({w,h},init_val)
{

}

template <typename T>
image2d_t<T>::image2d_t( const data_t &other )
:data(other)
{

}

template <typename T>
image2d_t<T>::image2d_t( data_t &&other ) noexcept
:data(std::move(other))
{

}

template <typename T>
image2d_t<T>::image2d_t( image2d_t &&other ) noexcept
:data(std::move(other.data))
{

}

template <typename T>
image2d_t<T> &image2d_t<T>::operator=( image2d_t &&other ) noexcept
{
	data = std::move(other.data);
	return *this;
}

template <typename T>
void image2d_t<T>::initialize( int w, int h, uninitialized_t )
{
	*this = image2d_t<T>({w,h},uninitialized_t{});
}

template <typename T>
void image2d_t<T>::initialize( int w, int h, const T *src )
{
	*this = image2d_t<T>({w,h},src);
}

template <typename T>
void image2d_t<T>::initialize( int w, int h, const T &init_val )
{
	*this = image2d_t<T>({w,h},init_val);
}

template <typename T>
void image2d_t<T>::swap( image2d_t &other )
{
	data.swap(other.data);
}

template <typename T>
void image2d_t<T>::destroy()
{
	data.destroy();
}

template <typename T>
bool image2d_t<T>::is_available() const noexcept
{
	return data.is_available();
}

template <typename T>
int image2d_t<T>::width() const noexcept
{
	return data.shape()[0];
}

template <typename T>
int image2d_t<T>::height() const noexcept
{
	return data.shape()[1];
}

template <typename T>
math::vec2i image2d_t<T>::size() const noexcept
{
	return {data.shape()[0],data.shape()[1]};
}

template <typename T>
void image2d_t<T>::clear( const T &value )
{
	data.map_inplace([&](T& v){
		v = value;
	});
}

template <typename T>
T &image2d_t<T>::operator()( int x, int y ) noexcept
{
	return data(x,y);
}

template <typename T>
const T &image2d_t<T>::operator()( int x, int y ) const noexcept
{
	return data(x,y);
}

template <typename T>
T &image2d_t<T>::at( int x, int y ) noexcept
{
	return data.at({x,y});
}

template <typename T>
const T &image2d_t<T>::at( int x, int y ) const noexcept
{
	return data.at({x,y});
}

template <typename T>
typename image2d_t<T>::data_t &image2d_t<T>::get_data() noexcept
{
	return data;
}

template <typename T>
const typename image2d_t<T>::data_t &image2d_t<T>::get_data() const noexcept
{
	return data;
}

template <typename T>
T *image2d_t<T>::get_raw_data() noexcept
{
	return data.raw_data();
}

template <typename T>
const T *image2d_t<T>::get_raw_data() const noexcept
{
	return data.raw_data();
}

template <typename T>
template <typename F>
auto image2d_t<T>::map( F &&f ) const
{
	using RT = remove_rcv_t<decltype(f(std::declval<T>()))>;
	return image2d_t<RT>(data.map(std::forward<F>(f)));
}

template <typename T>
template <typename F>
void image2d_t<T>::map_inplace( F &&f ) const
{
	data.map_inplace(std::forward<F>(f));
}

template <typename T>
image2d_t<T> image2d_t<T>::sub_image( int beg_x, int beg_y, int end_x, int end_y ) const
{
	assert(is_available());
	assert(beg_x >= 0 && beg_x < end_x && end_x <= width());
	assert(beg_y >= 0 && beg_y < end_y && end_y <= height());
	int x_size = end_x - beg_x;
	int y_size = end_y - beg_y;
	image2d_t<T> ret(x_size,y_size);
	for(int y = 0; y< y_size; ++y)
		for(int x = 0; x < x_size; ++x)
			ret(x,y) = at(beg_x + x,beg_y + y);
	return ret;
}

template <typename T>
image2d_t<T> image2d_t<T>::flip_vertically() const
{
	image2d_t<T> ret(width(),height(),uninitialized_t{});
	for(int y = 0; y < height(); ++y){
		int oy = height() - 1 - y;
		for(int x = 0; x < width(); ++x)
			ret(x,y) = at(x,oy);
	}
	return ret;
}

template <typename T>
void image2d_t<T>::flip_vertically_inplace()
{
	for(int y = 0; y < height() / 2; ++y){
		int oy = height() - 1 - y;
		for(int x = 0; x < width(); ++x)
			std::swap(at(x,y),at(x,oy));
	}
}

template <typename T>
image2d_t<T> image2d_t<T>::flip_horizontally() const
{
	image2d_t<T> ret(width(),height(),uninitialized_t{});
	for(int y = 0; y < height(); ++y){
		for(int x = 0; x < width(); ++x)
			ret(x,y) = at(width() - 1 - x, y);
	}
	return ret;
}

template <typename T>
void image2d_t<T>::flip_horizontally_inplace()
{
	for(int y = 0; y < height(); ++y){
		for(int x = 0; x < width() / 2; ++x)
			swap(at(x,y),at(width() -1 - x,y));
	}
}

template <typename T>
image2d_view_t<T, false> image2d_t<T>::sub_view( int beg_x, int beg_y, int end_x, int end_y )
{
	return image2d_view_t<T, false>(data.get_subview({beg_x,beg_y},{end_x-beg_x,end_y-beg_y}));
}

template <typename T>
image2d_view_t<T, true> image2d_t<T>::sub_view( int beg_x, int beg_y, int end_x, int end_y ) const
{
	return image2d_view_t<T, true>(data.get_subview({beg_x,beg_y},{end_x-beg_x,end_y-beg_y}));
}

template <typename T>
image2d_view_t<T, true> image2d_t<T>::sub_view_const( int beg_x, int beg_y, int end_x, int end_y ) const
{
	return image2d_view_t<T, true>(data.get_const_subview({beg_x,beg_y},{end_x-beg_x,end_y-beg_y}));
}

template <typename T>
template <typename U>
auto image2d_t<T>::operator+( const image2d_t<U> &other ) const
{
	assert(is_available() && other.is_available());
	assert(size() == other.size());

	using RT = remove_rcv_t<decltype(std::declval<T>() + std::declval<U>())>;
	image2d_t<RT> ret(width(),height());
	for(int y = 0; y < height(); ++y){
		for(int x = 0; x < width(); ++x){
			ret(x,y) = operator()(x,y) + other(x,y);
		}
	}
	return ret;
}

template <typename T>
template <typename U>
auto image2d_t<T>::operator*( const image2d_t<U> &other ) const
{
	assert(is_available() && other.is_available());
	assert(size() == other.size());

	using RT = remove_rcv_t<decltype(std::declval<T>() * std::declval<U>())>;
	image2d_t<RT> ret(width(),height());
	for(int y = 0; y < height(); ++y){
		for(int x = 0; x < width(); ++x){
			ret(x,y) = operator()(x,y) * other(x,y);
		}
	}
	return ret;
}

template <typename T>
template <typename U>
image2d_t<T> &image2d_t<T>::operator+=( const image2d_t<U> &other ) const
{
	assert(is_available() && other.is_available());
	assert(size() == other.size());

	for(int y = 0; y < height(); ++y){
		for(int x = 0; x < width(); ++x){
			at(x,y) += other(x,y);
		}
	}
	return *this;
}

template <typename T>
template <typename U>
image2d_t<T> &image2d_t<T>::operator*=( const image2d_t<U> &other ) const
{
	assert(is_available() && other.is_available());
	assert(size() == other.size());

	for(int y = 0; y < height(); ++y){
		for(int x = 0; x < width(); ++x){
			at(x,y) *= other(x,y);
		}
	}
	return *this;
}

}
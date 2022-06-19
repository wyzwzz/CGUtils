#pragma once

namespace wzz::texture{

template <typename T>
image1d_t<T>::image1d_t( int w, uninitialized_t )
:data({w},uninitialized_t{})
{

}

template <typename T>
image1d_t<T>::image1d_t( int w, const T *src )
:data(data_t::from_linear_array({w},src))
{

}

template <typename T>
image1d_t<T>::image1d_t( int w, const T& init_val)
:data(math::tvec<int,1>{w},init_val)
{

}

template <typename T>
image1d_t<T>::image1d_t( const data_t &other )
:data(other)
{

}

template <typename T>
image1d_t<T>::image1d_t( data_t &&other ) noexcept
:data(std::move(other))
{

}

template <typename T>
image1d_t<T>::image1d_t( image1d_t &&other ) noexcept
:data(std::move(other.data))
{

}

template <typename T>
image1d_t<T> &image1d_t<T>::operator=( image1d_t &&other ) noexcept
{
	data = std::move(other);
	return *this;
}
template <typename T>
void image1d_t<T>::initialize( int w, uninitialized_t )
{
	*this = image1d_t<T>(w,uninitialized_t{});
}
template <typename T>
void image1d_t<T>::initialize( int w, const T *src )
{
	*this = image1d_t<T>(w,src);
}
template <typename T>
void image1d_t<T>::initialize( int w, const T &init_val )
{
	*this = image1d_t<T>(w,init_val);
}

template <typename T>
void image1d_t<T>::swap(image1d_t & other)
{
	data.swap(other.data);
}

template <typename T>
void image1d_t<T>::destroy()
{
	data.destroy();
}
template <typename T>
bool image1d_t<T>::is_available() const noexcept
{
	return data.is_available();
}
template <typename T>
int image1d_t<T>::width() const noexcept
{
	return data.shape()[0];
}
template <typename T>
int image1d_t<T>::size() const noexcept
{
	return data.shape()[0];
}
template <typename T>
void image1d_t<T>::clear( const T &value )
{
	if(!is_available()) return;
	data.map_inplace([&](T& v){
		v = value;
	});
}
template <typename T>
T &image1d_t<T>::operator()( int x ) noexcept
{
	return data[x];
}
template <typename T>
const T &image1d_t<T>::operator()( int x ) const noexcept
{
	return data[x];
}
template <typename T>
T &image1d_t<T>::at( int x ) noexcept
{
	return data[x];
}
template <typename T>
const T &image1d_t<T>::at( int x ) const noexcept
{
	return data[x];
}
template <typename T>
typename image1d_t<T>::data_t &image1d_t<T>::get_data() noexcept
{
	return data;
}
template <typename T>
const typename image1d_t<T>::data_t &image1d_t<T>::get_data() const noexcept
{
	return data;
}
template <typename T>
T *image1d_t<T>::get_raw_data() noexcept
{
	return data.raw_data();
}
template <typename T>
const T *image1d_t<T>::get_raw_data() const noexcept
{
	return data.raw_data();
}
template <typename T>
template <typename F>
auto image1d_t<T>::map( F &&f ) const
{
	using RT = remove_rcv_t<decltype(f(std::declval<T>()))>;
	return image1d_t<RT>(data.map(std::forward<F>(f)));
}

template <typename T>
template <typename F>
void image1d_t<T>::map_inplace( F &&f ) const
{
	data.map_inplace(std::forward<F>(f));
}

template <typename T>
image1d_t<T> image1d_t<T>::sub_image( int beg_x, int end_x ) const
{
	assert(is_available());
	assert(beg_x >= 0 && beg_x < end_x && end_x <= width());
	int x_size = end_x - beg_x;
	image1d_t<T> ret(x_size);
	for(int x = 0; x < x_size; ++x)
		ret(x) = at(beg_x + x);
	return ret;
}

template <typename T>
image1d_t<T> image1d_t<T>::flip_horizontally() const
{
	image1d_t<T> ret = *this;
	int w = width();
	for(int i = 0; i < w / 2; i++){
		swap(ret(i),ret(w - 1 - i));
	}
	return ret;
}

template <typename T>
void image1d_t<T>::flip_horizontally_inplace()
{
	int w = width();
	int hw = w / 2;
	for(int i = 0; i < hw; i++){
		swap(at(i),at(w - 1 - i));
	}
}
template <typename T>
image1d_view_t<T, false> image1d_t<T>::sub_view( int beg_x, int end_x )
{
	return image1d_view_t<T,false>(data.get_subview(math::tvec<int,1>{beg_x},math::tvec<int,1>{end_x - beg_x}));
}

template <typename T>
image1d_view_t<T, true> image1d_t<T>::sub_view( int beg_x, int end_x ) const
{
	return image1d_view_t<T,true>(data.get_subview(math::tvec<int,1>{beg_x},math::tvec<int,1>{end_x - beg_x}));
}

template <typename T>
image1d_view_t<T, true> image1d_t<T>::sub_view_const( int beg_x, int end_x ) const
{
	return image1d_view_t<T,true>(data.get_const_subview(math::tvec<int,1>{beg_x},math::tvec<int,1>{end_x - beg_x}));
}

template <typename T>
template <typename U>
auto image1d_t<T>::operator+( const image1d_t<U> &other ) const
{
	assert(this->is_available() && other.is_available());
	assert(this->data.shape() == other->data.shape());

	using RT = remove_rcv_t<decltype(std::declval<T>() + std::declval<U>())>;
	image1d_t<RT> ret(width());
	for(int x = 0; x < width(); ++x){
		ret(x) = this->at(x) + other->at(x);
	}
	return ret;
}

template <typename T>
template <typename U>
auto image1d_t<T>::operator*( const image1d_t<U> &other ) const
{
	assert(this->is_available() && other.is_available());
	assert(this->data.shape() == other->data.shape());

	using RT = remove_rcv_t<decltype(std::declval<T>() * std::declval<U>())>;
	image1d_t<RT> ret(width());
	for(int x = 0; x < width(); ++x){
		ret(x) = this->at(x) * other->at(x);
	}
	return ret;
}

template <typename T>
template <typename U>
image1d_t<T> &image1d_t<T>::operator+=( const image1d_t<U> &other ) const
{
	assert(this->is_available() && other.is_available());
	assert(this->data.shape() == other->data.shape());

	for(int x = 0; x < width(); ++x){
		this->at(x) += other->at(x);
	}
	return *this;
}

template <typename T>
template <typename U>
image1d_t<T> &image1d_t<T>::operator*=( const image1d_t<U> &other ) const
{
	assert(this->is_available() && other.is_available());
	assert(this->data.shape() == other->data.shape());

	for(int x = 0; x < width(); ++x){
		this->at(x) *= other->at(x);
	}
	return *this;
}


}
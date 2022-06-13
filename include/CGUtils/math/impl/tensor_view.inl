#pragma once

#include <CGUtils/math/decl/tensor_view.hpp>

WZZ_MATH_BEGIN

template <typename T, int D, bool CONST>
tensor_view_t<T, D, CONST>::tensor_view_t()
:data(nullptr),origin(0),shape_(0)
{

}

template <typename T, int D, bool CONST>
tensor_view_t<T, D, CONST>::tensor_view_t( data_t data, const index_t &view_origin, const index_t &view_shape ) noexcept
:data(data),origin(view_origin),shape_(view_shape)
{

}

template <typename T, int D, bool CONST>
const typename tensor_view_t<T,D,CONST>::index_t &tensor_view_t<T, D, CONST>::shape() const noexcept
{
	return shape_;
}

template <typename T, int D, bool CONST>
auto &tensor_view_t<T, D, CONST>::at( const index_t &index ) noexcept
{
	return data->at(origin + index);
}

template <typename T, int D, bool CONST>
auto &tensor_view_t<T, D, CONST>::at( const index_t &index ) const noexcept
{
	return data->at(origin + index);
}

template <typename T, int D, bool CONST>
template <typename... Args, typename>
auto &tensor_view_t<T, D, CONST>::operator()( Args... indices ) noexcept
{
	return at({indices...});
}

template <typename T, int D, bool CONST>
template <typename... Args, typename>
auto &tensor_view_t<T, D, CONST>::operator()( Args... indices ) const noexcept
{
	return at({indices...});
}

template <typename T, int D, bool CONST>
void tensor_view_t<T, D, CONST>::swap( tensor_view_t::self_t &other ) noexcept
{
	std::swap(data,  other.data);
	std::swap(shape_, other.shape_);
	std::swap(origin,  other.origin);
}

template <typename T, int D, bool CONST>
typename tensor_view_t<T,D,CONST>::self_t tensor_view_t<T, D, CONST>::get_subview( const index_t &origin, const index_t &shape )
{
	return self_t(data,this->origin + origin,shape);
}

template <typename T, int D, bool CONST>
tensor_view_t<T, D, true> tensor_view_t<T, D, CONST>::get_subview( const index_t &origin, const index_t &shape ) const
{
	return tensor_view_t<T, D, true>(data,this->origin + origin,shape);
}

template <typename T, int D, bool CONST>
typename tensor_view_t<T,D,CONST>::self_t tensor_view_t<T, D, CONST>::get_const_subview( const index_t &origin, const index_t &shape ) const
{
	return tensor_view_t<T,D,true>(data,this->origin + origin, shape);
}

WZZ_MATH_END
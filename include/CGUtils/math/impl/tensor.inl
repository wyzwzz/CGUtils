#pragma once

#include "CGUtils/math/tensor.hpp"
#include <CGUtils/memory/alloc.hpp>
#include <CGUtils/misc/scope_bomb.hpp>
#include <cassert>
WZZ_MATH_BEGIN

template <typename T, int D>
tensor_t<T, D>::tensor_t()
:data(nullptr),shape_(0),elem_cnt(0)
{
}

template <typename T, int D>
tensor_t<T, D>::tensor_t( const tensor_t::index_t &shape, uninitialized_t )
:data(nullptr),shape_(shape),elem_cnt(shape.product())
{
	data = std::allocator<T>().allocate(elem_cnt);
}

template <typename T, int D>
tensor_t<T, D>::tensor_t( const tensor_t::index_t &shape, const T &init_val )
:tensor_t(shape,uninitialized_t{})
{
	for(size_t i = 0; i < elem_cnt; ++i)
		new(data + i) T(init_val);
}

template <typename T, int D>
template <typename F>
tensor_t<T, D>::tensor_t( const tensor_t::index_t &shape, F &&f )
:data(nullptr),shape_(shape),elem_cnt(shape.product())
{
	assert(elem_cnt > 0);
	int constructed_count = 0;
	T* d = std::allocator<T>().allocate(elem_cnt);
	//delete if new try any exceptions
	misc::scope_bomb_t bomb([&](){
		alloc::call_destructor(d,constructed_count);
		std::allocator<T>().deallocate(d,elem_cnt);
	});
	for(size_t i = 0; i < elem_cnt; ++i,++constructed_count)
		new(d+i) T(f(i));
	data = d;
	bomb.dismiss();
}

template <typename T, int D>
template <typename F>
typename tensor_t<T,D>::self_t tensor_t<T, D>::from_linear_index_func( const index_t& shape, F &&f )
{
	return tensor_t<T,D>(shape,std::forward<F>(f));
}

template <typename T, int D>
typename tensor_t<T,D>::self_t tensor_t<T, D>::from_linear_array( const tensor_t::index_t &shape, const T *d )
{
	return from_linear_index_func(shape,[d](int i){
		  return d[i];
	});
}

template <typename T, int D>
tensor_t<T, D>::tensor_t( const tensor_t::self_t &copy_from )
:tensor_t()
{
	if(copy_from.is_available()){
		*this = tensor_t::from_linear_index_func(copy_from.shape_,[&](int i){
			return copy_from.data[i];
		});
	}
}

template <typename T, int D>
tensor_t<T, D>::tensor_t( tensor_t::self_t &&move_from ) noexcept
:tensor_t()
{
	swap(move_from);
}

template <typename T, int D>
tensor_t<T, D> &tensor_t<T, D>::operator=( const tensor_t::self_t &copy_from )
{
	if(shape_ != copy_from.shape_){
		self_t t(copy_from);
		this->swap(t);
		return *this;
	}

	assert(elem_cnt == copy_from.elem_cnt);
	for(size_t i = 0; i < elem_cnt; ++i)
		data[i] = copy_from.data[i];
	return *this;
}

template <typename T, int D>
tensor_t<T, D> &tensor_t<T, D>::operator=( tensor_t::self_t &&move_from ) noexcept
{
	this->swap(move_from);
	return *this;
}

template <typename T, int D>
tensor_t<T, D>::~tensor_t()
{
	destroy();
}

template <typename T, int D>
void tensor_t<T, D>::initialize( const tensor_t::index_t &shape, const T &init_value )
{
	self_t t(shape,init_value);
	swap(t);
}

template <typename T, int D>
bool tensor_t<T, D>::is_available() const noexcept
{
	return data;
}

template <typename T, int D>
void tensor_t<T, D>::destruct()
{
	assert(data);
	alloc::call_destructor(data,elem_cnt);
}

template <typename T, int D>
void tensor_t<T, D>::destroy()
{
	if(data)
	{
		assert(elem_cnt > 0);

		this->destruct();
		std::allocator<T>().deallocate(data, elem_cnt);
		data = nullptr;
	}
	shape_ = index_t(0);
	elem_cnt = 0;
}

template <typename T, int D>
const typename tensor_t<T,D>::index_t &tensor_t<T, D>::shape() const noexcept
{
	return shape_;
}

template <typename T, int D>
size_t tensor_t<T, D>::elem_count() const noexcept
{
	return elem_cnt;
}

template <typename T, int D>
size_t tensor_t<T, D>::to_linear_index(const index_t& index) const noexcept
{
	if constexpr (D ==  1){
		return index[0];
	}
	else if constexpr (D == 2){
		//(x,y) (w,h)
		return index[0] + index[1] * shape_[0];
	}
	else if constexpr (D == 3 ){
		return index[0] + index[1] * shape_[0] + index[2] * shape_[0] * shape_[1];
	}
	else{
		size_t idx = 0, t = 1;
		for(int i = 0; i < D; i++){
			idx += index[0] * t;
			t *= shape_[i];
		}
		return idx;
	}
}

template <typename T, int D>
T &tensor_t<T, D>::at( const tensor_t::index_t &index ) noexcept
{
	assert(is_available());
	return data[to_linear_index(index)];
}

template <typename T, int D>
const T &tensor_t<T, D>::at( const tensor_t::index_t &index ) const noexcept
{
	assert(is_available());
	return data[ to_linear_index(index)];
}

template <typename T, int D>
T &tensor_t<T, D>::operator[]( size_t index ) noexcept
{
	assert(is_available() && index < elem_cnt);
	return data[index];
}

template <typename T, int D>
const T &tensor_t<T, D>::operator[]( size_t index ) const noexcept
{
	assert(is_available() && index < elem_cnt);
	return data[index];
}

template <typename T, int D>
template <typename... Args, typename>
T &tensor_t<T, D>::operator()( Args... indices ) noexcept
{
	return at({int(indices)...});
}

template <typename T, int D>
template <typename... Args, typename>
const T &tensor_t<T, D>::operator()( Args... indices ) const noexcept
{
	return at({int(indices)...});
}

template <typename T, int D>
void tensor_t<T, D>::swap( tensor_t::self_t &swap_with ) noexcept
{
	std::swap(data,swap_with.data);
	std::swap(shape_,swap_with.shape_);
	std::swap(elem_cnt,swap_with.elem_cnt);
}

template <typename T, int D>
template <typename F>
auto tensor_t<T, D>::map( F &&func ) const
{
	using RT = remove_rcv_t<decltype(func(T(0)))>;
	if(!is_available())
		return tensor_t<RT,D>();

	return tensor_t<RT,D>::from_linear_index_func([&](int i){
		return func(data[i]);
	});
}

template <typename T, int D>
template <typename F>
void tensor_t<T, D>::map_inplace( F &&func ) const
{
	if(!is_available())
		return;
	for(size_t i = 0; i < elem_cnt; ++i)
		func(data[i]);
}

template <typename T, int D>
T *tensor_t<T, D>::raw_data() noexcept
{
	return data;
}

template <typename T, int D>
const T *tensor_t<T, D>::raw_data() const noexcept
{
	return data;
}

template <typename T, int D>
size_t tensor_t<T, D>::raw_data_size_bytes() const noexcept
{
	return elem_cnt * sizeof(T);
}

template <typename T, int D>
tensor_view_t<T, D, false> tensor_t<T, D>::get_subview( const index_t &origin, const index_t &shape )
{
 	return tensor_view_t<T, D, false>(this,origin,shape);
}

template <typename T, int D>
tensor_view_t<T, D, true> tensor_t<T, D>::get_subview( const tensor_t::index_t &origin, const tensor_t::index_t &shape ) const
{
	return tensor_view_t<T, D, true>(this,origin,shape);
}

template <typename T, int D>
tensor_view_t<T, D, true> tensor_t<T, D>::get_const_subview( const tensor_t::index_t &origin, const tensor_t::index_t &shape ) const
{
	return tensor_view_t<T, D, true>(this,origin,shape);
}

//===

template<typename T, int D, typename F>
auto elemwise_unary(const tensor_t<T, D> &opd, F &&opr){
	return opd.map(std::forward<F>(opr));
}

template<typename T, int D, typename F>
auto elemwise_binary(const tensor_t<T, D> &lhs, const tensor_t<T, D> &rhs, F &&opr){
	if(lhs.shape() != rhs.shape() || !lhs.is_available()){
		throw std::runtime_error("invalid shape for tensor binary operation");
	}
	using RT = remove_rcv_t<decltype(opr(lhs[0],rhs[0]))>;
	return tensor_t<RT,D>::from_linear_index_func(lhs.shape(),[&](int i){
		return opr(lhs[i],rhs[i]);
	});
}

template<typename T, int D>
bool operator==(const tensor_t<T, D> &lhs, const tensor_t<T, D> &rhs){
	if(lhs.shape() != rhs.shape() || !lhs.is_available()){
		throw std::runtime_error("invalid shape for tensor binary operation");
	}
	size_t cnt = lhs.elem_count();
	for(size_t i = 0; i < cnt; ++i)
		if(lhs[i] != rhs[i])
			return false;
	return true;
}

template<typename T, int D>
bool operator!=(const tensor_t<T, D> &lhs, const tensor_t<T, D> &rhs){
	if(lhs.shape() != rhs.shape() || !lhs.is_available()){
		throw std::runtime_error("invalid shape for tensor binary operation");
	}
	size_t cnt = lhs.elem_count();
	for(size_t i = 0; i < cnt; ++i)
		if(lhs[i] != rhs[i])
			return true;
	return false;
}

template<typename T, int D>
auto operator+(const tensor_t<T, D> &lhs, const tensor_t<T, D> &rhs){
	return elemwise_binary(lhs,rhs,std::plus<T>());
}

template<typename T, int D>
auto operator-(const tensor_t<T, D> &lhs, const tensor_t<T, D> &rhs){
	return elemwise_binary(lhs,rhs,std::minus<T>());
}

template<typename T, int D>
auto operator*(const tensor_t<T, D> &lhs, const tensor_t<T, D> &rhs){
	return elemwise_binary(lhs,rhs,std::multiplies<T>());
}

template<typename T, int D>
auto operator/(const tensor_t<T, D> &lhs, const tensor_t<T, D> &rhs){
	return elemwise_binary(lhs,rhs,std::divides<T>());
}

WZZ_MATH_END
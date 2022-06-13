#pragma once

#include "vec.hpp"

WZZ_MATH_BEGIN

template<typename T, int D>
class tensor_t;

template <typename T,int D,bool CONST>
class tensor_view_t{
	using data_t = std::conditional_t<CONST,const tensor_t<T,D>*,tensor_t<T,D>*>;
	data_t data;

	vec<int,D> origin;//start coord
	vec<int,D> shape_;//shape of view

public:
	using index_t = vec<int,D>;
	using self_t = tensor_view_t<T,D,CONST>;

	static constexpr int Dim = D;

	tensor_view_t();
	tensor_view_t(data_t data,const index_t& view_origin,const index_t& view_shape) noexcept;

	const index_t& shape() const noexcept;
	auto& at(const index_t& index) noexcept;
	auto& at(const index_t& index) const noexcept;

	template <typename...Args,typename = std::enable_if_t<sizeof...(Args) == D>>
	auto& operator()(Args... indices) noexcept;

	template <typename...Args,typename = std::enable_if_t<sizeof...(Args) == D>>
	auto& operator()(Args... indices) const noexcept;

	void swap(self_t& other) noexcept;

	self_t get_subview(const index_t& origin,const index_t& shape);

	tensor_view_t<T,D,true> get_subview(const index_t& origin,const index_t& shape) const;

	self_t get_const_subview(const index_t& origin,const index_t& shape) const;

	auto get_data() noexcept {return data;}
	auto get_data() const noexcept {return data;}

};


WZZ_MATH_END
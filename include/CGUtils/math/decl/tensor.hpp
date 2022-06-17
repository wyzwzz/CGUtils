//
// Created by wyz on 2022/6/10.
//
#pragma once
#include "common.hpp"
#include "tensor_view.hpp"

WZZ_MATH_BEGIN

template <typename T,int D>
class tensor_t{
public:
	using index_t = vec<int,D>;
	using self_t = tensor_t<T,D>;
	static constexpr int Dim = D;
private:
	T* data = nullptr;
	vec<int,D> shape_;//D0, D1, D2, D3, D4... or width, height, depth
	size_t elem_cnt = 0;

	template<typename F>
	tensor_t(const index_t& shape,F&& f);

	void destruct();

	size_t to_linear_index(const index_t& index) const noexcept;
public:


	tensor_t();
	tensor_t(const index_t &shape,uninitialized_t);
	explicit tensor_t(const index_t& shape,const T& init_val = T());

	template <typename F>
	static self_t from_linear_index_func(const index_t& shape,F&& f);

	static self_t from_linear_array(const index_t& shape,const T* d);

	tensor_t(const self_t &copy_from);
	tensor_t(self_t &&move_from) noexcept;

	tensor_t<T, D> &operator=(const self_t &copy_from);
	tensor_t<T, D> &operator=(self_t &&move_from) noexcept;

	~tensor_t();

	void initialize(const index_t &shape, const T &init_value = T());

	bool is_available() const noexcept;

	void destroy();

	const index_t &shape() const noexcept;

	size_t elem_count() const noexcept;

	T& at(const index_t& index) noexcept;
	const T& at(const index_t& index) const noexcept;

	T& operator[](size_t index) noexcept;
	const T& operator[](size_t index) const noexcept;

	template<typename...Args, typename = std::enable_if_t<sizeof...(Args)==D>>
	T &operator()(Args...indices) noexcept;

	template<typename...Args,typename = std::enable_if_t<sizeof...(Args) == D>>
	const T &operator()(Args...indices) const noexcept;

	void swap(self_t &swap_with) noexcept;

	template<typename F>
	auto map(F &&func) const;

	//use ref in F
	template<typename F>
	void map_inplace(F &&func) const;

	T *raw_data() noexcept;
	const T *raw_data() const noexcept;

	size_t raw_data_size_bytes() const noexcept;

	tensor_view_t<T, D, false> get_subview(const index_t &origin,const index_t &shape);

	tensor_view_t<T, D, true> get_subview(const index_t &origin,const index_t &shape) const;

	tensor_view_t<T, D, true> get_const_subview(const index_t &origin,const index_t &shape) const;
};

template<typename T, int D, typename F>
auto elemwise_unary(const tensor_t<T, D> &opd, F &&opr);

template<typename T, int D, typename F>
auto elemwise_binary(
  const tensor_t<T, D> &lhs, const tensor_t<T, D> &rhs, F &&opr);

template<typename T, int D>
bool operator==(const tensor_t<T, D> &lhs, const tensor_t<T, D> &rhs);

template<typename T, int D>
bool operator!=(const tensor_t<T, D> &lhs, const tensor_t<T, D> &rhs);

template<typename T, int D>
auto operator+(const tensor_t<T, D> &lhs, const tensor_t<T, D> &rhs);

template<typename T, int D>
auto operator-(const tensor_t<T, D> &lhs, const tensor_t<T, D> &rhs);

template<typename T, int D>
auto operator*(const tensor_t<T, D> &lhs, const tensor_t<T, D> &rhs);

template<typename T, int D>
auto operator/(const tensor_t<T, D> &lhs, const tensor_t<T, D> &rhs);

WZZ_MATH_END

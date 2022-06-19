#pragma once

#include "../math.hpp"

namespace wzz::texture{

template <typename T,bool CONST>
class image1d_view_t{
public:
	using data_t = math::tensor_view_t<T,1,CONST>;

	image1d_view_t() = default;

	explicit image1d_view_t(data_t data);

	void swap(image1d_view_t& other) noexcept;

	int width() const noexcept;
	int size() const noexcept;

	T& operator()(int x) noexcept;
	const T& operator()(int x) const noexcept;

	T& at(int x) noexcept;
	const T& at(int x) const noexcept;

	data_t& get_data() noexcept;
	const data_t& get_data() const noexcept;

	image1d_view_t sub_view(int beg_x,int end_x);

	image1d_view_t<T,true> sub_view(int beg_x,int end_x) const;

	image1d_view_t<T,true> sub_view_const(int beg_x,int end_x) const;
private:
	data_t data;
};

}

#include "./impl/image1d_view.inl"
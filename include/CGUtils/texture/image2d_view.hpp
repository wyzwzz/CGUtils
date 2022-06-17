#pragma once

#include "../math.hpp"

namespace wzz::texture{

template <typename T,bool CONST>
class image2d_view_t{
public:
	using data_t = math::tensor_view_t<T,2,CONST>;

	image2d_view_t() = default;

	explicit image2d_view_t(data_t data);

	int width() const noexcept;
	int height() const noexcept;
	math::vec2i size() const noexcept;

	T& operator()(int x,int y) noexcept;
	const T& operator()(int x,int y) const noexcept;

	T& at(int x,int y) noexcept;
	const T& at(int x,int y) const noexcept;

	data_t& get_data() noexcept;
	const data_t& get_data() const noexcept;

	image2d_view_t sub_view(int beg_x,int beg_y,int end_x,int end_y);

	image2d_view_t<T,true> sub_view(int beg_x,int beg_y,int end_x,int end_y) const;

	image2d_view_t<T,true> sub_view_const(int beg_x,int beg_y,int end_x,int end_y) const;
private:
	data_t data;
};

}
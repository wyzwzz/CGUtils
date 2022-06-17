#pragma once
#include "../math.hpp"

namespace wzz::texture{

template <typename T,bool CONST>
class image3d_view_t{
public:
	using data_t = math::tensor_view_t<T,3,CONST>;

	image3d_view_t() = default;

	explicit image3d_view_t(data_t data);

	int width() const noexcept;
	int height() const noexcept;
	int depth() const noexcept;
	math::vec3i size() const noexcept;

	T& operator()(int x,int y,int z) noexcept;
	const T& operator()(int x,int y,int z) const noexcept;

	T& at(int x,int y,int z) noexcept;
	const T& at(int x,int y,int z) const noexcept;

	data_t& get_data() noexcept;
	const data_t& get_data() const noexcept;

	image3d_view_t sub_view(int beg_x,int beg_y,int beg_z,int end_x,int end_y,int end_z);

	image3d_view_t<T,true> sub_view(int beg_x,int beg_y,int beg_z,int end_x,int end_y,int end_z) const;

	image3d_view_t<T,true> sub_view_const(int beg_x,int beg_y,int beg_z,int end_x,int end_y,int end_z) const;
private:
	data_t data;
};


}
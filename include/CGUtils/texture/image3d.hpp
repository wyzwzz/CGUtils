#pragma once
#include "../math.hpp"
#include "image3d_view.hpp"

namespace wzz::texture{

template <typename T>
class image3d_t{
public:
	using data_t = math::tensor_t<T,3>;

	image3d_t() = default;
	image3d_t(int w,int h,int d,uninitialized_t);
	image3d_t(int w,int h,int d,const T* src);
	image3d_t(int w,int h,int d,const T& init_val = T());

	explicit image3d_t(const data_t& other);
	explicit image3d_t(data_t&& other) noexcept;

	image3d_t(const image3d_t&) = default;
	image3d_t& operator=(const image3d_t&) = default;

	image3d_t(image3d_t&& other) noexcept;
	image3d_t& operator=(image3d_t&& other) noexcept;

	~image3d_t() = default;

	void initialize(int w,int h,int d,uninitialized_t);
	void initialize(int w,int h,int d,const T* src);
	void initialize(int w,int h,int d,const T& init_val = T());

	void swap(image3d_t& other);

	void destroy();

	bool is_available() const noexcept;

	int width() const noexcept;
	int height() const noexcept;
	int depth() const noexcept;
	math::vec3i size() const noexcept;

	void clear(const T& value = T());


	T& operator()(int x,int y,int z) noexcept;
	const T& operator()(int x,int y,int z) const noexcept;

	T& at(int x,int y,int z) noexcept;
	const T& at(int x,int y,int z) const noexcept;

	data_t& get_data() noexcept;
	const data_t& get_data() const noexcept;

	T* get_raw_data() noexcept;
	const T* get_raw_data() const noexcept;

	template <typename F>
	auto map(F&& f) const;

	template <typename F>
	void map_inplace(F&& f) const;

	image3d_t sub_image(int beg_x,int beg_y,int beg_z,int end_x,int end_y,int end_z) const;

	image3d_view_t<T,false> sub_view(int beg_x,int beg_y,int beg_z,int end_x,int end_y,int end_z);

	image3d_view_t<T,true> sub_view(int beg_x,int beg_y,int beg_z,int end_x,int end_y,int end_z) const;

	image3d_view_t<T,true> sub_view_const(int beg_x,int beg_y,int beg_z,int end_x,int end_y,int end_z) const;

	template <typename U>
	auto operator+(const image3d_t<U>& other) const;

	template <typename U>
	auto operator*(const image3d_t<U>& other) const;

	template <typename U>
	image3d_t& operator+=(const image3d_t<U>& other) const;

	template <typename U>
	image3d_t& operator*=(const image3d_t<U>& other) const;
protected:
	data_t data;
};
}

#include "./impl/image3d.inl"
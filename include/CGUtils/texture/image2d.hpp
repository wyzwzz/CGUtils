#pragma once
#include "../math.hpp"
#include "image2d_view.hpp"

namespace wzz::texture{

template <typename T>
class image2d_t{
public:
	using data_t = math::tensor_t<T,2>;

	image2d_t() = default;
	image2d_t(int w,int h,uninitialized_t);
	image2d_t(int w,int h,const T* src);
	image2d_t(int w,int h,const T& init_val = T());

	explicit image2d_t(const data_t& other);
	explicit image2d_t(data_t&& other) noexcept;

	image2d_t(const image2d_t&) = default;
	image2d_t& operator=(const image2d_t&) = default;

	image2d_t(image2d_t&& other) noexcept;
	image2d_t& operator=(image2d_t&& other) noexcept;

	~image2d_t() = default;

	void initialize(int w,int h,uninitialized_t);
	void initialize(int w,int h,const T* src);
	void initialize(int w,int h,const T& init_val = T());

	void swap(image2d_t& other);

	void destroy();

	bool is_available() const noexcept;

	int width() const noexcept;
	int height() const noexcept;
	math::vec2i size() const noexcept;

	void clear(const T& value = T());


	T& operator()(int x,int y) noexcept;
	const T& operator()(int x,int y) const noexcept;

	T& at(int x,int y) noexcept;
	const T& at(int x,int y) const noexcept;

	data_t& get_data() noexcept;
	const data_t& get_data() const noexcept;

	T* get_raw_data() noexcept;
	const T* get_raw_data() const noexcept;

	template <typename F>
	auto map(F&& f) const;

	template <typename F>
	void map_inplace(F&& f) const;

	image2d_t sub_image(int beg_x,int beg_y,int end_x,int end_y) const;


	image2d_t flip_vertically() const;

	void flip_vertically_inplace();

	image2d_t flip_horizontally() const;

	void flip_horizontally_inplace();


	image2d_view_t<T,false> sub_view(int beg_x,int beg_y,int end_x,int end_y);

	image2d_view_t<T,true> sub_view(int beg_x,int beg_y,int end_x,int end_y) const;

	image2d_view_t<T,true> sub_view_const(int beg_x,int beg_y,int end_x,int end_y) const;

	template <typename U>
	auto operator+(const image2d_t<U>& other) const;

	template <typename U>
	auto operator*(const image2d_t<U>& other) const;

	template <typename U>
	image2d_t& operator+=(const image2d_t<U>& other) const;

	template <typename U>
	image2d_t& operator*=(const image2d_t<U>& other) const;
protected:
	data_t data;
};
}

#include "./impl/image2d.inl"
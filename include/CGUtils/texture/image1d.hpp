#pragma once

#include "image1d_view.hpp"

namespace wzz::texture{

template <typename T>
class image1d_t{
public:
	using data_t = math::tensor_t<T,1>;

	image1d_t() = default;
	image1d_t(int w,uninitialized_t);
	image1d_t(int w,const T* src);
	explicit image1d_t(int w,const T& init_val = T());

	explicit image1d_t(const data_t& other);
	explicit image1d_t(data_t&& other) noexcept;

	image1d_t(const image1d_t&) = default;
	image1d_t& operator=(const image1d_t&) = default;

	image1d_t(image1d_t&& other) noexcept;
	image1d_t& operator=(image1d_t&& other) noexcept;

	~image1d_t() = default;

	void initialize(int w,uninitialized_t);
	void initialize(int w,const T* src);
	void initialize(int w,const T& init_val = T());

	void swap(image1d_t& other);

	void destroy();

	bool is_available() const noexcept;

	int width() const noexcept;
	int size() const noexcept;

	void clear(const T& value = T());


	T& operator()(int x) noexcept;
	const T& operator()(int x) const noexcept;

	T& at(int x) noexcept;
	const T& at(int x) const noexcept;

	data_t& get_data() noexcept;
	const data_t& get_data() const noexcept;

	T* get_raw_data() noexcept;
	const T* get_raw_data() const noexcept;

	template <typename F>
	auto map(F&& f) const;

	template <typename F>
	void map_inplace(F&& f) const;

	image1d_t sub_image(int beg_x,int end_x) const;

	image1d_t flip_horizontally() const;

	void flip_horizontally_inplace();


	image1d_view_t<T,false> sub_view(int beg_x,int end_x);

	image1d_view_t<T,true> sub_view(int beg_x,int end_x) const;

	image1d_view_t<T,true> sub_view_const(int beg_x,int end_x) const;

	template <typename U>
	auto operator+(const image1d_t<U>& other) const;

	template <typename U>
	auto operator*(const image1d_t<U>& other) const;

	template <typename U>
	image1d_t& operator+=(const image1d_t<U>& other) const;

	template <typename U>
	image1d_t& operator*=(const image1d_t<U>& other) const;
protected:
	data_t data;
};
}

#include "./impl/image1d.inl"
#pragma once

#include "image2d.hpp"

namespace wzz::texture{

template <typename T,typename F = math::avg_value_t<T>>
class mipmap2d_t{
public:
	mipmap2d_t();

	explicit mipmap2d_t(const image2d_t<T>& lod0_image);
	explicit mipmap2d_t(image2d_t<T>&& lod0_image);

	void generate(const image2d_t<T>& lod0_image);
	void generate(image2d_t<T>&& lod0_image);

	bool available() const noexcept;

	void destroy();

	int levels() const noexcept;

	image2d_t<T>& get_level(int level) noexcept;

	const image2d_t<T>& get_level(int level) const noexcept;

private:
	std::vector<image2d_t<T>> images;
};

}

#include "./impl/mipmap2d.inl"
#pragma once

#include "image3d.hpp"

namespace wzz::texture{

template <typename T,typename F = math::avg_value_t<T>>
class mipmap3d_t{
public:
	mipmap3d_t();
	
	explicit mipmap3d_t(const image3d_t<T>& lod0_image);
	explicit mipmap3d_t(image3d_t<T>&& lod0_image);
	
	void generate(const image3d_t<T>& lod0_image);
	void generate(image3d_t<T>&& lod0_image);
	
	bool available() const noexcept;
	
	void destroy();
	
	int levels() const noexcept;
	
	image3d_t<T>& get_level(int level) noexcept;
	
	const image3d_t<T>& get_level(int level) const noexcept;
	
private:
	std::vector<image3d_t<T>> images;
};


}


#include "./impl/mipmap3d.inl"
#pragma once
#include <map>
#include "../math.hpp"
#include "image1d.hpp"
namespace wzz::texture{

template <typename T,typename std::enable_if_t<std::is_floating_point_v<T>>>
class color_mapping1d_t{
public:
	color_mapping1d_t() = default;

	//todo
	using color_point_t = std::pair<T,math::tcolor4<T>>;
	template<color_point_t>
	void add_point(const color_point_t& color){
		add_point(color.first,color.second);
	}
	template <color_point_t,typename... Args>
	void add_point(const color_point_t& color,Args... colors){
		add_point(color.first,color.second);
		add_point<Args...>(colors...);
	}

	void add_point(T s,const math::tcolor4<T>& color);

	void clear();

	image1d_t<T> get_alpha_map(int interval = 255) const;

	image1d_t<math::tcolor3<T>> get_rgb_map(int interval = 255) const;

	image1d_t<math::tcolor4<T>> get_rgba_map(int interval = 255) const;

private:
	std::map<T,math::tcolor4<T>> mp;
};


}
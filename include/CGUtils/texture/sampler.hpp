#pragma once

#include "../math.hpp"
#include "image1d.hpp"
#include "image2d.hpp"
#include "image3d.hpp"
namespace wzz::texture{

struct linear_sampler_t{
	template <typename T,typename Tex>
	static auto sample1d(T u,Tex&& tex,int width)
	-> decltype(tex(std::declval<int>()))
	{

	}

	template<typename T,typename F = float>
	static auto sample1d(const image1d_t<T>& image,F u){

	}

	/**
	 * @param Tex function<Texel(int x,int y)>
	 */
	template <typename T,typename Tex>
	static auto sample2d(const math::tvec2<T>& uv,Tex&& tex,int width,int height)
	->decltype(tex(std::declval<int>(),std::declval<int>()))
	{

	}

	template<typename T,typename F = float>
	static auto sample2d(const image2d_t<T>& image,const math::tvec2<F>& uv){

	}

	/**
	 * @param Tex function<Texel(int x,int y,int z)>
	 */
	template <typename T,typename Tex>
	static auto sample3d(const math::tvec3<T>& uvw,Tex&& tex,int width,int height,int depth)
	  ->decltype(tex(std::declval<int>(),std::declval<int>(),std::declval<int>()))
	{

	}

	template<typename T,typename F = float>
	static auto sample3d(const image2d_t<T>& image,const math::tvec3<F>& uvw){

	}
};

struct nearest_sampler_t{
	template <typename T,typename Tex>
	static auto sample1d(T u,Tex&& tex,int width)
	  -> decltype(tex(std::declval<int>()))
	{

	}

	template<typename T,typename F = float>
	static auto sample1d(const image1d_t<T>& image,F u){

	}

	/**
	 * @param Tex function<Texel(int x,int y)>
	 */
	template <typename T,typename Tex>
	static auto sample2d(const math::tvec2<T>& uv,Tex&& tex,int width,int height)
	  ->decltype(tex(std::declval<int>(),std::declval<int>()))
	{

	}

	template<typename T,typename F = float>
	static auto sample2d(const image2d_t<T>& image,const math::tvec2<F>& uv){

	}

	/**
	 * @param Tex function<Texel(int x,int y,int z)>
	 */
	template <typename T,typename Tex>
	static auto sample3d(const math::tvec3<T>& uvw,Tex&& tex,int width,int height,int depth)
	  ->decltype(tex(std::declval<int>(),std::declval<int>(),std::declval<int>()))
	{

	}

	template<typename T,typename F = float>
	static auto sample3d(const image2d_t<T>& image,const math::tvec3<F>& uvw){

	}
};


}
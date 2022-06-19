#pragma once

#include "../math.hpp"
#include "image1d.hpp"
#include "image2d.hpp"
#include "image3d.hpp"
namespace wzz::texture{

struct linear_sampler_t{
	template <typename T,typename Tex>
	static auto sample1d(T u,Tex&& tex,int width)
	-> decltype(tex(std::declval<int>()))//todo ???
	{
		const auto fu = static_cast<T>(width * u);
		const int pu = std::clamp<int>(static_cast<int>(fu),0,width - 1);

		const int dpu = fu > pu + T(0.5) ? 1 : -1;

		const int apu = std::clamp(pu + dpu,0, width - 1);

		const auto du = std::min<T>(std::abs(fu - (pu + T(0.5))),1);

		return tex(pu) * (T(1) - du) + du * tex(apu);
	}

	template<typename T,typename F = float>
	static auto sample1d(const image1d_t<T>& image,F u){
		return sample1d(u,[&](int x){
			return image.at(x);
		},image.width());
	}

	/**
	 * @param Tex function<Texel(int x,int y)>
	 */
	template <typename T,typename Tex>
	static auto sample2d(const math::tvec2<T>& uv,Tex&& tex,int width,int height)
	->decltype(tex(std::declval<int>(),std::declval<int>()))
	{
		const auto fu = static_cast<T>(uv.x * width);
		const auto fv = static_cast<T>(uv.y * height);

		const int pu = math::clamp(static_cast<int>(fu), 0, width - 1);
		const int pv = math::clamp(static_cast<int>(fv), 0, height - 1);

		const int dpu = (fu > pu + T(0.5)) ? 1 : -1;
		const int dpv = (fv > pv + T(0.5)) ? 1 : -1;

		const int apu = math::clamp(pu + dpu, 0, width - 1);
		const int apv = math::clamp(pv + dpv, 0, height - 1);

		const T du = (std::min)(std::abs(fu - pu - T(0.5)), T(1));
		const T dv = (std::min)(std::abs(fv - pv - T(0.5)), T(1));

		const auto pupv   = tex(pu,  pv);
		const auto apupv  = tex(apu, pv);
		const auto puapv  = tex(pu,  apv);
		const auto apuapv = tex(apu, apv);

		return (pupv * (1 - du) + apupv * du) * (1 - dv) +
			   (puapv * (1 - du) + apuapv * du) * dv;
	}

	template<typename T,typename F = float>
	static auto sample2d(const image2d_t<T>& image,const math::tvec2<F>& uv){
		return sample2d(uv,[&](int x,int y){
			  return image(x,y);
		},image.width(),image.height());
	}

	/**
	 * @param Tex function<Texel(int x,int y,int z)>
	 */
	template <typename T,typename Tex>
	static auto sample3d(const math::tvec3<T>& uvw,Tex&& tex,int width,int height,int depth)
	  ->decltype(tex(std::declval<int>(),std::declval<int>(),std::declval<int>()))
	{
		const auto fu = static_cast<T>(uvw.x * width);
		const auto fv = static_cast<T>(uvw.y * height);
		const auto fw = static_cast<T>(uvw.z * depth);

		const int pu = math::clamp(static_cast<int>(fu), 0, width - 1);
		const int pv = math::clamp(static_cast<int>(fv), 0, height - 1);
		const int pw = math::clamp(static_cast<int>(fw), 0, depth - 1);

		const int dpu = (fu > pu + T(0.5)) ? 1 : -1;
		const int dpv = (fv > pv + T(0.5)) ? 1 : -1;
		const int dpw = (fw > pw + T(0.5)) ? 1 : -1;

		const int apu = math::clamp(pu + dpu, 0, width - 1);
		const int apv = math::clamp(pv + dpv, 0, height - 1);
		const int apw = math::clamp(pw + dpw, 0, depth - 1);

		const T du = (std::min)(std::abs(fu - pu - T(0.5)), T(1));
		const T dv = (std::min)(std::abs(fv - pv - T(0.5)), T(1));
		const T dw = (std::min)(std::abs(fw - pw - T(0.5)), T(1));

		const auto pupvpw   = tex(pu,  pv, pw);
		const auto apupvpw  = tex(apu, pv, pw);
		const auto puapvpw  = tex(pu,  apv, pw);
		const auto apuapvpw = tex(apu, apv, pw);
		const auto pupvapw   = tex(pu,  pv, apw);
		const auto apupvapw  = tex(apu, pv, apw);
		const auto puapvapw  = tex(pu,  apv, apw);
		const auto apuapvapw = tex(apu, apv, apw);

		return ((pupvpw * (1 - du) + apupvpw * du) * (1 - dv) +
			   (puapvpw * (1 - du) + apuapvpw * du) * dv) * (1 - dw) +
			   ((pupvapw * (1 - du) + apupvapw * du) * (1 - dv) +
			   (puapvapw * (1 - du) + apuapvapw * du) * dv) * dw;
	}

	template<typename T,typename F = float>
	static auto sample3d(const image3d_t<T>& image,const math::tvec3<F>& uvw){
		return sample3d(uvw,[&](int x,int y,int z){
			return image(x,y,z);
		},image.width(),image.height(),image.depth());
	}
};

struct nearest_sampler_t{
	template <typename T,typename Tex>
	static auto sample1d(T u,Tex&& tex,int width)
	  -> decltype(tex(std::declval<int>()))
	{
		const int x = std::clamp<int>(u * width,0,width - 1);
		return tex(x);
	}

	template<typename T,typename F = float>
	static auto sample1d(const image1d_t<T>& image,F u){
		const int x = std::clamp<int>(u * image.width(), 0, image.width() - 1);
		return image.at(x);
	}

	/**
	 * @param Tex function<Texel(int x,int y)>
	 */
	template <typename T,typename Tex>
	static auto sample2d(const math::tvec2<T>& uv,Tex&& tex,int width,int height)
	  ->decltype(tex(std::declval<int>(),std::declval<int>()))
	{
		const int x = std::clamp<int>(uv.x * width, 0, width - 1);
		const int y = std::clamp<int>(uv.y * height, 0, height - 1);
		return tex(x,y);
	}

	template<typename T,typename F = float>
	static auto sample2d(const image2d_t<T>& image,const math::tvec2<F>& uv){
		const int x = std::clamp<int>(uv.x * image.width(), 0, image.width() - 1);
		const int y = std::clamp<int>(uv.y * image.height(), 0, image.height() - 1);
		return image(x,y);
	}

	/**
	 * @param Tex function<Texel(int x,int y,int z)>
	 */
	template <typename T,typename Tex>
	static auto sample3d(const math::tvec3<T>& uvw,Tex&& tex,int width,int height,int depth)
	  ->decltype(tex(std::declval<int>(),std::declval<int>(),std::declval<int>()))
	{
		const int x = std::clamp<int>(uvw.x * width, 0, width - 1);
		const int y = std::clamp<int>(uvw.y * height, 0, height - 1);
		const int z = std::clamp<int>(uvw.z * depth, 0, depth - 1);
		return tex(x,y,z);
	}

	template<typename T,typename F = float>
	static auto sample3d(const image2d_t<T>& image,const math::tvec3<F>& uvw){
		const int x = std::clamp<int>(uvw.x * image.width(), 0, image.width() - 1);
		const int y = std::clamp<int>(uvw.y * image.height(), 0, image.height() - 1);
		const int z = std::clamp<int>(uvw.z * image.depth(), 0, image.depth() - 1);
		return image(x,y,z);
	}
};


}
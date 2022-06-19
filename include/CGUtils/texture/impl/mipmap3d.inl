#pragma once

namespace wzz::texture{

template <typename T, typename F>
mipmap3d_t<T, F>::mipmap3d_t()
{
}

template <typename T, typename F>
mipmap3d_t<T, F>::mipmap3d_t( const image3d_t<T> &lod0_image )
{
	generate(lod0_image);
}

template <typename T, typename F>
mipmap3d_t<T, F>::mipmap3d_t( image3d_t<T>&& lod0_image )
{
	generate(std::move(lod0_image));
}

template <typename T, typename F>
void mipmap3d_t<T, F>::generate( const image3d_t<T> &lod0_image )
{
	auto t = lod0_image;
	generate(std::move(t));
}
template <typename T, typename F>
void mipmap3d_t<T, F>::generate( image3d_t<T>&& lod0_image )
{
	destroy();
	images.emplace_back(std::move(lod0_image));
	auto& _last_lod_image = images.back();
	int last_w = _last_lod_image.width();
	int last_h = _last_lod_image.height();
	int last_d = _last_lod_image.depth();

	while(last_w > 1 && last_h > 1 && last_d > 1){
		const int cur_w = (last_w + 1) >> 1;
		const int cur_h = (last_h + 1) >> 1;
		const int cur_d = (last_d + 1) >> 1;
		image3d_t<T> cur_lod_image(cur_w,cur_h,cur_d);
		auto& last_lod_image = images.back();
		auto f = F();
		for(int z = 0; z < cur_d; ++z){
			const int lz = z << 1;
			for(int y = 0; y < cur_h; ++y){
				const int ly = y << 1;
				for(int x = 0; x < cur_w; ++x){
					const int lx = x << 1;
					const auto t000 = last_lod_image(lx, ly,lz);
					const auto t001 = (lx + 1 < last_w) ? last_lod_image(lx + 1, ly, lz) : T();
					const auto t010 = (ly + 1 < last_h) ? last_lod_image(lx, ly + 1, lz) : T();
					const auto t011 = (lx + 1 < last_w && ly + 1 < last_h) ? last_lod_image(lx + 1, ly + 1, lz) : T();
					const auto t100 = (lz + 1 < last_d) ? last_lod_image(lx,ly,lz + 1) : T();
					const auto t101 = (lz + 1 < last_d && lx + 1 < last_w) ? last_lod_image(lx + 1, ly, lz + 1) : T();
					const auto t110 = (lz + 1 < last_d && ly + 1 < last_h) ? last_lod_image(lx, ly + 1, lz + 1) : T();
					const auto t111 = (lz + 1 < last_d && lx + 1 < last_w && ly + 1 < last_h) ? last_lod_image(lx + 1, ly + 1, lz + 1) : T();
					const auto v0 = f(f(t000,t001),f(t010,t011));
					const auto v1 = f(f(t100,t101),f(t110,t111));
					cur_lod_image(x,y,z) = f(v0,v1);
				}
			}
		}
		images.emplace_back(std::move(cur_lod_image));
		last_w = cur_w;
		last_h = cur_h;
		last_d = cur_d;
	}
}

template <typename T, typename F>
bool mipmap3d_t<T, F>::available() const noexcept
{
	return !images.empty();
}

template <typename T, typename F>
void mipmap3d_t<T, F>::destroy()
{
	images.clear();
}

template <typename T, typename F>
int mipmap3d_t<T, F>::levels() const noexcept
{
	return images.size();
}

template <typename T, typename F>
image3d_t<T> &mipmap3d_t<T, F>::get_level( int level ) noexcept
{
	assert(available());
	return images.at(level);
}

template <typename T, typename F>
const image3d_t<T> &mipmap3d_t<T, F>::get_level( int level ) const noexcept
{
	assert(available());
	return images.at(level);
}

}
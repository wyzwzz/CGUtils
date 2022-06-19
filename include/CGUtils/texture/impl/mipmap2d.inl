#pragma once

namespace wzz::texture{

template <typename T,typename F>
mipmap2d_t<T,F>::mipmap2d_t()
{

}

template <typename T,typename F>
mipmap2d_t<T,F>::mipmap2d_t( const image2d_t<T> &lod0_image )
{
	generate(lod0_image);
}

template <typename T,typename F>
mipmap2d_t<T,F>::mipmap2d_t( image2d_t<T>&& lod0_image )
{
	generate(std::move(lod0_image));
}

template <typename T,typename F>
void mipmap2d_t<T,F>::generate( const image2d_t<T> &lod0_image )
{
	auto t = lod0_image;
	generate(std::move(t));
}

template <typename T,typename F>
void mipmap2d_t<T,F>::generate( image2d_t<T>&& lod0_image )
{
	destroy();
	images.emplace_back(std::move(lod0_image));
	auto& _last_lod_image = images.back();
	int last_w = _last_lod_image.width();
	int last_h = _last_lod_image.height();
	auto f = F();
	while(last_w > 1 && last_h > 1){
		const int cur_w = (last_w + 1) >> 1;
		const int cur_h = (last_h + 1) >> 1;
		image2d_t<T> cur_lod_image(cur_w,cur_h);
		auto& last_lod_image = images.back();
		for(int y = 0; y < cur_h; ++y){
			const int ly = y << 1;
			for(int x = 0; x < cur_w; ++x){
				const int lx = x << 1;
				const auto t00 = last_lod_image(lx, ly);
				const auto t01 = (lx + 1 >= last_w) ? T() : last_lod_image(lx + 1, ly);
				const auto t10 = (ly + 1 >= last_h) ? T() : last_lod_image(lx, ly + 1);
				const auto t11 = (lx + 1 < last_w && ly + 1 < last_h) ? last_lod_image(lx + 1, ly + 1) : T();
				cur_lod_image(x,y) = f(f(t00,t01),f(t10,t11));
			}
		}
		images.emplace_back(std::move(cur_lod_image));
		last_w = cur_w;
		last_h = cur_h;
	}
}

template <typename T,typename F>
bool mipmap2d_t<T,F>::available() const noexcept
{
	return !images.empty();
}

template <typename T,typename F>
void mipmap2d_t<T,F>::destroy()
{
	images.clear();
}

template <typename T,typename F>
int mipmap2d_t<T,F>::levels() const noexcept
{
	return images.size();
}

template <typename T,typename F>
image2d_t<T> &mipmap2d_t<T,F>::get_level( int level ) noexcept
{
	assert(available());
	return images.at(level);
}

template <typename T,typename F>
const image2d_t<T> &mipmap2d_t<T,F>::get_level( int level ) const noexcept
{
	assert(available());
	return images.at(level);
}

}
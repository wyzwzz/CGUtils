#include <gtest/gtest.h>
#include <CGUtils/texture.hpp>
#include <CGUtils/console.hpp>
using namespace wzz;
TEST(test_texture,texture_image1d){
	texture::image1d_t<math::vec3f> image0(32,math::vec3f(1.f));
	int idx = 0;
	image0.map_inplace([&](math::vec3f& v){
		v.x = idx++;
		v.y = idx++;
		v.z = v.x + v.y;
	});
	for(int i = 0; i < 32; i++){
		EXPECT_FLOAT_EQ(image0(i).x,i * 2);
		EXPECT_FLOAT_EQ(image0(i).y,2 * i + 1);
		EXPECT_FLOAT_EQ(image0(i).z,4 * i + 1);
	}
	auto view = image0.sub_view(12,26);
	for(int i = 12;i < 16;i++){
		EXPECT_FLOAT_EQ(view(i-12).x,i*2);
		EXPECT_FLOAT_EQ(view(i-12).y,2 * i + 1);
		EXPECT_FLOAT_EQ(view(i-12).z,4 * i + 1);
	}
}

TEST(test_texture,texture_image2d){
	texture::image2d_t<math::vec3f> image0(32,32,math::vec3f(1.f));

	auto view = image0.sub_view(12,12,16,16);
	LOG_INFO("view size: {} {}",view.width(),view.height());
	auto sub_img = image0.sub_image(9,9,12,12);
	texture::mipmap2d_t<math::vec3f,math::avg_value_t<math::vec3f>> mipmap;
	auto image1 = image0;
	mipmap.generate(image0);

}

TEST(test_texture,texture_image3d){
	texture::image3d_t<math::vec3f> image0(16,16,16,math::vec3f(1.f));

	auto view = image0.sub_view(12,12,12,16,16,16);

	auto sub_img = image0.sub_image(8,8,8,10,10,10);

	auto v = texture::linear_sampler_t::sample3d(image0,math::vec3f(1.5,1.5,1.5));
	EXPECT_FLOAT_EQ(v.x,1.f);
	EXPECT_FLOAT_EQ(v.y,1.f);
	EXPECT_FLOAT_EQ(v.z,1.f);

	auto image2 = image0;
	texture::mipmap3d_t<math::vec3f,math::max_value_t<math::vec3f>> mipmap;
	mipmap.generate(std::move(image2));

}

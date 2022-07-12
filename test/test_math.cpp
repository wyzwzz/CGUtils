//
// Created by wyz on 2022/2/16.
//
#include <CGUtils/math.hpp>
#include <tuple>
#include <iostream>
#include <CGUtils/misc.hpp>
#include <CGUtils/time/timer.hpp>
#include <gtest/gtest.h>

using namespace wzz::math;

TEST(test_math,test_color2){
	color2f c;
	c.rg();
	c.rr();
	c.gg();
	c.gr();
}

TEST(test_math,test_color3){
	color3f c(1,2,3);
	auto c2 = c.rgb();
	EXPECT_EQ(c,c2);
	auto c3 = c.bgr();
	c3 = c3.bgr();
	EXPECT_EQ(c3,c2);
}

TEST(test_math,test_color4){
	color4f c(1,2,3,4);
	auto c2 = c.rgba();
	EXPECT_EQ(c,c2);
	auto c3 = c.abgr().abgr();
	EXPECT_EQ(c3,c);
}

TEST(test_math,test_vec3){
    vec3f v1(1,2,3);
    v1.normalize();
    EXPECT_FLOAT_EQ(v1.length(),1);
	v1.map([](const vec3f &v){
		return vec3d(v.x,v.y,v.z);
	});
	auto v2 = v1.xxx();
}

TEST(test_math,test_vec){
	vec<float,5> v1({1,2,3,4,5});
	vec<float,5> v2(5,4,3,2,1);
	vec<int,1> vv({1});
	vec<int,5> v33({1,2,3,4,5});
	v1 += v2;
	vec<float,5> v3(6,6,6,6,6);
	for(int i = 0;i < 5;i++)
		EXPECT_FLOAT_EQ(v1[i],6);
	v1.normalize();
	EXPECT_FLOAT_EQ(v1.length(),1);
}

TEST(test_math,test_tensor){
	tensor_t<vec3f,2> hdr_image({20,10});
	for(int y = 0; y < hdr_image.shape()[1]; ++y){
		for(int x = 0; x < hdr_image.shape()[0]; ++x){
			hdr_image(x,y) = vec3f(x + 1, y + 1, x + y);
		}
	}
	hdr_image.map_inplace([](vec3f& v){
		v.normalize();
	});
	std::cout<<"hdr image elem_count: "<<hdr_image.elem_count()<<std::endl;
	for(int i = 0; i < hdr_image.elem_count(); ++i){
		auto& v= hdr_image[i];
		EXPECT_FLOAT_EQ(v.length(),1.f);
	}
	auto image2 = std::move(hdr_image);
	EXPECT_EQ(hdr_image.shape()[0],0);
	EXPECT_EQ(hdr_image.shape()[1],0);
	EXPECT_FALSE(hdr_image.is_available());
	EXPECT_EQ(image2.shape()[0],20);
	EXPECT_EQ(image2.shape()[1],10);
	for(int i = 0; i < image2.elem_count(); ++i){
		auto& v= image2[i];
		EXPECT_FLOAT_EQ(v.length(),1.f);
	}

	auto view = image2.get_subview(vec<int,2>{10,5},vec<int,2>{10,5});
	EXPECT_EQ(view.shape()[0],10);
	EXPECT_EQ(view.shape()[1],5);

}

TEST(test_math,test_mat4_c_and_transform){
	auto r = mat4f_c::left_transform::translate(1,2,3);
	auto r1 = transform3f::translate(1,2,3);
	auto r2 = transform3f::translate(-1,2,3);
	r(2,1) = 5;
	auto rr = r;
	std::cout<<rr.get_row(0)<<std::endl;
	std::cout<<rr.get_row(1)<<std::endl;
	std::cout<<rr.get_row(2)<<std::endl;
	std::cout<<rr.get_row(3)<<std::endl;
	vec3f p(1,-2,-3);
	p = r2.apply_to_point(p);
	EXPECT_FLOAT_EQ(p.x,0);
	EXPECT_FLOAT_EQ(p.y,0);
	EXPECT_FLOAT_EQ(p.z,0);
	p = r1.apply_to_point(p);
	EXPECT_FLOAT_EQ(p.x,1);
	EXPECT_FLOAT_EQ(p.y,2);
	EXPECT_FLOAT_EQ(p.z,3);
	vec3f d(1,0,0);
	auto r3 = transform3f::rotate_z(PI_f);
	d = r3.apply_to_vector(d);
	EXPECT_FLOAT_EQ(d.x,-1.f);
	EXPECT_NEAR(d.y,0.f,0.00001);
	EXPECT_FLOAT_EQ(d.z,0.f);
	std::cout<<d<<std::endl;
}

TEST(test_math,test_sh){

	auto res = sh::project_func_to_sh<float,3>([](float phi,float theta)noexcept ->float{
		return 1.f;
	},512);
	std::cout<<res<<std::endl;
}
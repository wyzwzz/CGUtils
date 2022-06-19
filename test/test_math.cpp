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

TEST(test_math,test_vec3){
    vec3f v1(1,2,3);
    v1.normalize();
    EXPECT_FLOAT_EQ(v1.length(),1);
	v1.map([](const vec3f &v){
		return vec3d(v.x,v.y,v.z);
	});

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

//
// Created by wyz on 2022/2/16.
//
#include <CGUtils/math.hpp>
#include <tuple>
#include <iostream>
#include <CGUtils/misc.hpp>
#include <CGUtils/timer.hpp>
using namespace wzz;
class A{};

int main(){
    int length = 1024;
    std::vector<int> count(length,0);
    for(int l = 0;l<11;l++){
        for(int z = 0;z<12;z++){
            for(int y= 0;y<12;y++){
                for(int x = 0;x<12;x++){
                    size_t v = misc::hash(x,y,z,l);
                    count[v%length]++;
                }
            }
        }
    }
    for(int i = 0;i<length;i++){
        std::cout<<"i "<<count[i]<<std::endl;
    }
    math::simd_float3_t sv1(1,1,1);
    math::simd_float3_t sv2(1,1,1);
    math::vec3f v1(1.1,1.1,1.1);
    math::vec3f v2(1.1,1.1,1.1);
    {
        AutoTimer t("test vec3f");
        for (int i = 0; i < 100000000; ++i)
        {
            v1 = v1 * v2;
        }
    }
    {
        AutoTimer t("test simd_float3");
        for(int i = 0; i < 100000000; ++i){
            sv2 = sv1 + sv2;
        }
    }

    return 0;
}
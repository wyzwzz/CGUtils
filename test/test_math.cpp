//
// Created by wyz on 2022/2/16.
//
#include <CGUtils/math.hpp>
#include <tuple>
#include <iostream>
int main(){
    int a=2,b=4,c=3;
    int x=2,y=4,z=4;
    std::cout<< (std::tie(a,b,c) < std::tie(x,y,z)) <<std::endl;
    return 0;
}
//
// Created by wyz on 2021/7/8.
//

#include <iostream>
#include <utility>

class A{

};
int main(){
    A a;
    std::cout<<typeid(std::ref(a)).name()<<std::endl;


    return 0;
}
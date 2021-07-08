//
// Created by wyz on 2021/7/8.
//
#include <CGUtils/LRU.hpp>
#include <CGUtils/hash_function.hpp>
#include <array>
#include <iostream>

using Array4=std::array<uint32_t,4>;
int main(){
    LRUCache<std::array<uint32_t,4>,int,Hash_UInt32Array4> cache(6);
    Array4 a1={1,2,3,4};
    Array4 a2={2,3,4,5};
    Array4 a3={1,2,3,1};
    Array4 a4={1,2,5,4};
    Array4 a5={1,6,3,4};
    Array4 a6={7,2,3,4};
    Array4 a7={11,22,3,4};
    Array4 a8={1,22,33,7};
    cache.emplace_back(a1,1);
    std::cout<<"insert after a1: "<<cache.front_value().value()<<std::endl;
    cache.emplace_back(a2,2);
    std::cout<<"insert after a2: "<<cache.front_value().value()<<std::endl;
    cache.emplace_back(a3,3);
    std::cout<<"insert after a3: "<<cache.front_value().value()<<std::endl;
    cache.emplace_back(a4,4);
    std::cout<<"insert after a4: "<<cache.front_value().value()<<std::endl;
    cache.emplace_back(a5,5);
    std::cout<<"insert after a5: "<<cache.front_value().value()<<std::endl;
    cache.emplace_back(a6,6);
    std::cout<<"insert after a6: "<<cache.front_value().value()<<std::endl;
    cache.emplace_back(a7,7);
    std::cout<<"insert after a7: "<<cache.front_value().value()<<std::endl;
    cache.emplace_back(a8,8);
    std::cout<<"insert after a8: "<<cache.front_value().value()<<std::endl;
    auto ptr=cache.get_value_ptr({1,1,1,1});
    if(!ptr)
        std::cout<<"ptr for 1,1,1,1 is: nullptr"<<std::endl;
    auto opt=cache.get_value({0,0,0,0});
    if(!opt.has_value()){
        std::cout<<"no value for 0,0,0,0"<<std::endl;
    }
    auto value=cache.get_value({1,2,3,4});
    if(!value.has_value()){
        std::cout<<"no cache for 1,2,3,4"<<std::endl;
    }
    value=cache.get_value({11,22,3,4});
    if(value.has_value()){
        std::cout<<"cache for 11 22 3 4"<<std::endl;
    }
}


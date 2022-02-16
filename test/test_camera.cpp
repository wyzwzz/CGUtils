//
// Created by wyz on 2021/7/5.
//
#include <CGUtils/camera.hpp>
#include <iostream>
#include <functional>
#include <any>
#include <type_traits>
template <int n>
auto compose(){
    if constexpr (n==1){
      return [&](auto&& f){
        return std::forward<decltype(f)>(f);
      };
    }
    else{
      auto call=[&](auto&& f){
        auto on=[&](auto&& g){
          return compose<n-1>()(
              [&](auto&& x){return f(g(std::forward<decltype(x)>(x)));}
          );
        };
        return on;
      };
      return call;
    }
}

int main(){

    auto add1=[](auto&& x){return x+1;};
    auto abs=[](auto&& x){ return std::abs(x);};
    auto ans=compose<3>()(abs)(add1)(add1)(-4.8);
    std::cout<<ans<<std::endl;
    std::cout<<typeid(ans).name()<<std::endl;

    std::cout<<sizeof(Camera)<<std::endl;
    return 0;
}

//
// Created by wyz on 2021/10/11.
//
#include <iostream>
#include "a.hpp"
int IntPow(int x,int y){
  int ans=1;
  for(int i=0;i<y;i++)
    ans *= x;
  return ans;
}
int main(){
  std::cout<<IntPow(2,6)<<std::endl;
  return 0;
}
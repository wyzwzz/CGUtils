//
// Created by wyz on 2021/7/8.
//
#include <iostream>
#include <thread>
#include <CGUtils/singleton.hpp>

class A{
public:
    A(){
        std::cout<<"A construct"<<std::endl;
    }
};
class B{
public:
    B(int x,int y){
        std::cout<<"B construct: "<<x<<" "<<y<<std::endl;
    }
    void print(){
        std::cout<<"print for B"<<std::endl;
    }
};
class C{
public:
    C(B&& _b){
        b=&_b;
    }
    B* b=nullptr;
};
int main(){
    Singleton<A>::get();

    std::thread t1([]{
        Singleton<B>::init(1,2);
    });
    std::thread t2([]{
        Singleton<B>::init(1,2);
    });
    std::thread t3([]{
        Singleton<B>::init(1,2);
    });
    t1.join();
    t2.join();
    t3.join();
    Singleton<B>::get()->print();
    B b(2,3);
    Singleton<C>::init(std::move(b));


}

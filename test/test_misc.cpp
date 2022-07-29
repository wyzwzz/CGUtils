#include <iostream>
#include <functional>
#include <any>
#include <type_traits>
#include <CGUtils/misc.hpp>
#include <gtest/gtest.h>

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
TEST(test_misc,test_compose){
    auto add1=[](auto&& x){return x+1;};
    auto abs=[](auto&& x){ return std::abs(x);};
    auto ans=compose<3>()(abs)(add1)(add1)(-4.8);
    std::cout<<ans<<std::endl;
    std::cout<<typeid(ans).name()<<std::endl;
}

using namespace wzz::misc;
struct A : intrusive_ptr_enabled_t<A>
{
	virtual ~A() = default;
	int a = 5;
	virtual void print() const{
		std::cout<<"a "<<a<<std::endl;
	}
};

struct B : A
{
	~B()
	{
		std::cout<<"destroy B"<<std::endl;
	}
	void print() const override{
		std::cout<<"b "<<b<<std::endl;
	}
	int b = 10;
};

TEST(test_misc,test_intrusive){
	std::vector<intrusive_ptr_t<A>> as;

	{
		auto b = make_handle<B>();
		intrusive_ptr_t<A> a;
		a = b;
		intrusive_ptr_t<A> c;
		c = a;
		auto bb = make_derived_handle<A,B>();
		bb->print();
		as.push_back(a);
	}
	std::cout<<"a->a = "<<as[0]->a<<std::endl;
}



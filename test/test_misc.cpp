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

static unsigned destructor_count = 0;

struct NonPOD : intrusive_hashmap_enabled_t<NonPOD>
{
	NonPOD(int a) { v = a; }
	~NonPOD()
	{
		destructor_count++;
	}
	int get() { return v; }
	int v;
};

static hash_t get_key(int v)
{
	return ((v & 7) << 24) | (v >> 3);
}

TEST(test_misc,test_hashmap){
	thread_safe_intrusive_hashmap_t<NonPOD> hash_map;

	for (int i = 0; i < 100000; i++)
	{
		hash_map.emplace_yield(get_key(i), i + 2000000);
		hash_map.emplace_replace(get_key(i), i + 3000000);
	}

	assert(destructor_count == 100000);

	for (int i = 0; i < 100000; i += 2)
	{
		hash_map.erase(hash_map.find(get_key(i)));
	}

	assert(destructor_count == 150000);

	for (int i = 1; i < 100000; i += 2)
	{
		auto *v = hash_map.find(get_key(i));
		assert(v && v->get() == i + 3000000);
	}

	hash_map.clear();
	assert(destructor_count == 200000);
}



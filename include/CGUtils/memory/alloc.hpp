#pragma once
#include <cassert>
#include <new>
#include <type_traits>

#include "../system//platform.hpp"


#ifndef WZZ_CC_MSVC
#include <cstdlib>
#endif

namespace wzz::alloc{


template <typename T>
void call_destructor(T& obj) noexcept{
	if constexpr (!std::is_trivially_destructible_v<T>){
		obj.~T();
	}
}
template <typename T>
void call_destructor(T* p,size_t n) noexcept{
	assert(p);
	for(size_t i = 0; i < n; ++i)
		call_destructor(p[i]);
}

inline constexpr size_t L1_CACHE_LINE_SIZE = 64;

inline void *aligned_alloc(size_t byte_size, size_t align)
{
	assert(align > 0 && !(align & (align - 1)));
#ifdef WZZ_CXX_MSVC

	void *ret = _aligned_malloc(byte_size, align);

#else

	void *ret = std::aligned_alloc(align, byte_size);

#endif

	if(!ret)
		throw std::bad_alloc();
	return ret;
}

template <typename T>
T* aligned_alloc(size_t count){
    return (T*) aligned_alloc(count * sizeof(T),L1_CACHE_LINE_SIZE);
}

inline void aligned_free(void *ptr)
{
	assert(ptr);

#ifdef WZZ_CXX_MSVC

	_aligned_free(ptr);

#else

	std::free(ptr);

#endif
}

template <typename T,size_t N>
class stack_allocator_t{
public:
	T* allocate_not_cared(size_t count){

	}

	using allocate = decltype(&stack_allocator_t<T,N>::allocate_not_cared);

	T* allocate_cleared(size_t count){
		T* ret = allocate_not_cared(count);
		if(ret)
			std::fill(ret,ret + count, T());
		return ret;
	}
	void reset(){
		offset = 0;
	}
private:
	T buffer[N];
	size_t offset = 0;
};

}
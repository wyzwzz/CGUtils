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

inline void aligned_free(void *ptr)
{
	assert(ptr);

#ifdef WZZ_CXX_MSVC

	_aligned_free(ptr);

#else

	std::free(ptr);

#endif
}

}
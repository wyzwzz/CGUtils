//
// Created by wyz on 2022/2/16.
//
#pragma once
#include <type_traits>
namespace wzz {

struct uninitialized_t {};

template <typename T>
using remove_rcv_t = std::remove_cv_t<std::remove_reference_t<T>>;

template<typename C, typename M>
ptrdiff_t byte_offset(M(C::*memPtr)) noexcept
{
	// 理论上这是UB，但我不知道有什么更好的方法
	return reinterpret_cast<char*>(&(reinterpret_cast<C*>(0)->*memPtr))
		   - reinterpret_cast<char*>(reinterpret_cast<C*>(0));
}

}
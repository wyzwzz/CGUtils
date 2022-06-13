//
// Created by wyz on 2022/2/16.
//
#pragma once
#include <type_traits>
namespace wzz {

struct uninitialized_t {};

template <typename T>
using remove_rcv_t = std::remove_cv_t<std::remove_reference_t<T>>;

}
#pragma once
#include "vec.hpp"
#include "vec3.hpp"
#include "mat3_c.hpp"
#include <functional>
namespace wzz::math{

namespace sh
{

/**
 * @brief evaluate the spherical harmonic basis funtion of degree L and oreder M
 * for the given direction dir
 */
template <typename T, int L, int M>
auto eval_sh( const tvec3<T> &dir ) noexcept;


template <typename T, int L, int M>
auto eval_sh(T phi, T theta) noexcept;


template <typename T>
using sh_expr_t = T(*)(const tvec3<T>&);


constexpr int get_linear_index( int L, int M ) noexcept { return L * (L + 1) + M; }


inline std::pair<int, int> get_order_and_degree( int index ) noexcept {
	const int L = static_cast<int>(std::sqrt(static_cast<float>(index)));
	const int M = index - L * ( L + 1);
	return {L,M};
}


template <typename T>
sh_expr_t<T> *sh_linear_table() noexcept;


template <typename T, int L>
tvec<T, 2 * L + 1> project_to_sh( const tvec3<T> &dir, T value ) noexcept;


template <typename T>
using sh_func_t = std::function<T( T phi, T theta)>;

constexpr int get_coef_count(int order) {
	return (order + 1) * (order + 1);
}

/**
 * @brief fit the given analytical spherical function to the SH basic functions up to order.
 * Using Monte Carlo method uniformly sampling the sphere.
 */
template <typename T, int L>
auto project_func_to_sh( const sh_func_t<T> &func, int sample_count );

template <typename T>
using rotate_func_t = std::function<void(const tmat3_c<T>&,T*) noexcept>;

template <typename T, int L>
void rotate_sh_coefs( const tmat3_c<T> &rot, T *coefs ) noexcept;


}

}
#pragma once

#include <functional>

#include "vec.hpp"
#include "vec3.hpp"
#include "mat3_c.hpp"

namespace wzz::math {

    namespace sh {

        //add #define NO_USE_HARD_CODED to use hard coded sh coefs compute

        /**
         * @brief evaluate the spherical harmonic basis funtion of degree L and oreder M
         * for the given direction dir
         */
        template<typename T, int L, int M>
        auto eval_sh(const tvec3<T> &dir) noexcept;


        template<typename T, int L, int M>
        auto eval_sh(T phi, T theta) noexcept;


        constexpr int get_coef_count(int degree) {
            return (degree + 1) * (degree + 1);
        }

        template<typename T>
        using sh_expr_t = T(*)(const tvec3<T> &);


        constexpr int get_linear_index(int L, int M) noexcept { return L * (L + 1) + M; }


        inline std::pair<int, int> get_order_and_degree(int index) noexcept {
            const int L = static_cast<int>(std::sqrt(static_cast<float>(index)));
            const int M = index - L * (L + 1);
            return {L, M};
        }


        template<typename T>
        sh_expr_t<T> *sh_linear_table() noexcept;


        template<int L, typename T>
        tvec<T, 2 * L + 1> project_to_sh(const tvec3<T> &dir, T value = T(1)) noexcept;


        template<typename T>
        using sh_func_t = std::function<T(T phi, T theta)>;

        /**
         * @brief fit the given analytical spherical function to the SH basic functions up to order.
         * Using Monte Carlo method uniformly sampling the sphere.
         */
        template<typename T, int L>
        tvec<T, get_coef_count(L)> project_func_to_sh(const sh_func_t<T> &func, int sample_count);


        template<typename T>
        std::vector<T> project_func_to_sh(int degree, const sh_func_t<T> &func, int sample_count);


        template<typename T, int DIM>
        std::vector<tvec<T,DIM>> project_func_to_sh(int degree, const std::function<tvec<T, DIM>(T, T)> &func, int sample_count);


        template<typename T>
        using rotate_func_t = void (*)(const tmat3_c<T> &, T *);


        template<typename T>
        void rotate_sh_coefs(const tmat3_c<T> &rot, int degree, T *coefs) noexcept;

    }

}
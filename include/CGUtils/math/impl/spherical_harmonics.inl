#pragma once
#include <random>
namespace wzz::math{

namespace sh{

namespace detail
{
//#define USE_HARD_CODED
template <typename T>
constexpr auto eval_sh_00(const tvec3<T>& dir){
#ifndef USE_HARD_CODED
	return T(0.5) * std::sqrt(invPI<T>);
#else
	return static_cast<T>(0.282095);
#endif
}

template <typename T>
auto eval_sh_1n1(const tvec3<T>& dir){
#ifndef USE_HARD_CODED
	return std::sqrt(T(0.75) * invPI<T>) * dir.y;
#else
	return static_cast<T>(-0.488603) * dir.y;
#endif
}

template <typename T>
auto eval_sh_10(const tvec3<T>& dir){
#ifndef USE_HARD_CODED
	return std::sqrt(T(0.75) * invPI<T>) * dir.z;
#else
	return T(0.488603) * dir.z;
#endif
}

template <typename T>
auto eval_sh_1p1(const tvec3<T>& dir){
#ifndef USE_HARD_CODED
	return std::sqrt(T(0.75) * invPI<T>) * dir.x;
#else
	return T(-0.488603) * dir.x;
#endif
}

template <typename T>
auto eval_sh_2n2(const tvec3<T>& dir){
#ifndef USE_HARD_CODED
	return  T(0.5) * std::sqrt(15 * invPI<T>) * dir.x * dir.y;
#else
	return T(1.092548) * dir.x * dir.y;
#endif
}

template <typename T>
auto eval_sh_2n1(const tvec3<T>& dir){
#ifndef USE_HARD_CODED
	return  T(0.5) * std::sqrt(15 * invPI<T>) * dir.y * dir.z;
#else
	return T(-1.092548) * dir.y * dir.z;
#endif
}

template <typename T>
auto eval_sh_20(const tvec3<T>& dir){
#ifndef USE_HARD_CODED
	return T(0.25) * std::sqrt(5 * invPI<T>) * (-dir.x * dir.x - dir.y * dir.y + 2 * dir.z * dir.z);
#else
	return T(0.315392) * (-dir.x * dir.x - dir.y * dir.y + 2 * dir.z * dir.z);
#endif
}

template <typename T>
auto eval_sh_2p1(const tvec3<T>& dir){
#ifndef USE_HARD_CODED
	return  T(0.5) * std::sqrt(15 * invPI<T>) * dir.x * dir.z;
#else
	return T(-1.092548) * dir.x * dir.z;
#endif
}

template <typename T>
auto eval_sh_2p2(const tvec3<T>& dir){
#ifndef USE_HARD_CODED
	return T(0.25) * std::sqrt(15 * invPI<T>) * (dir.x * dir.x - dir.y * dir.y);
#else
	return T(0.546274) * (dir.x * dir.x - dir.y * dir.y);
#endif
}

template <typename T>
auto eval_sh_3n3(const tvec3<T>& dir){
#ifndef USE_HARD_CODED
	return T(0.25) * std::sqrt(T(17.5) * invPI<T>) * dir.y * (3 * dir.x * dir.x - dir.y * dir.y);
#else
	return T(-0.590044) * dir.y * (3 * dir.x * dir.x - dir.y * dir.y);
#endif
}

template <typename T>
auto eval_sh_3n2(const tvec3<T>& dir){
#ifndef USE_HARD_CODED
	return T(0.5) * std::sqrt(105 * invPI<T>) * dir.x * dir.y * dir.z;
#else
	return T(2.890611) * dir.x * dir.y * dir.z;
#endif
}

template <typename T>
auto eval_sh_3n1(const tvec3<T>& dir){
#ifndef USE_HARD_CODED
	return T(0.25) * std::sqrt(T(10.5) * invPI<T>) * dir.y * (4 * dir.z * dir.z - dir.x * dir.x - dir.y * dir.y);
#else
	return T(-0.457046) * dir.y * (4 * dir.z * dir.z - dir.x * dir.x - dir.y * dir.y);
#endif
}

template <typename T>
auto eval_sh_30(const tvec3<T>& dir){
#ifndef USE_HARD_CODED
	return T(0.25) * std::sqrt(7 * invPI<T>) * dir.z * (2 * dir.z * dir.z - 3 * dir.x * dir.x - 3 * dir.y * dir.y);
#else
	return T(0.373176) * dir.z * (2 * dir.z * dir.z - 3 * dir.x * dir.x - 3 * dir.y * dir.y);
#endif
}

template <typename T>
auto eval_sh_3p1(const tvec3<T>& dir){
#ifndef USE_HARD_CODED
	return T(0.25) * std::sqrt(T(10.5) * invPI<T>) * dir.x * (4 * dir.z * dir.z - dir.x * dir.x - dir.y * dir.y);
#else
	return T(-0.457046) * dir.x * (4 * dir.z * dir.z - dir.x * dir.x - dir.y * dir.y);
#endif
}

template <typename T>
auto eval_sh_3p2(const tvec3<T>& dir){
#ifndef USE_HARD_CODED
	return T(0.25) * std::sqrt(105 * invPI<T>) * dir.z * (dir.x * dir.x - dir.y * dir.y);
#else
	return T(1.445306) * dir.z * (dir.x * dir.x - dir.y * dir.y);
#endif
}

template <typename T>
auto eval_sh_3p3(const tvec3<T>& dir){
#ifndef USE_HARD_CODED
	return T(0.25) * std::sqrt(T(17.5) * invPI<T>) * dir.x * (dir.x * dir.x - 3 * dir.y * dir.y);
#else
	return T(-0.590044) * dir.x * (dir.x * dir.x - 3 * dir.y * dir.y);;
#endif
}

template <typename T>
auto eval_sh_4n4(const tvec3<T>& dir){
#ifndef USE_HARD_CODED
	return T(0.75) * std::sqrt(35 * invPI<T>) * dir.x * dir.y * (dir.x * dir.x - dir.y * dir.y);
#else
	return T(2.503343) * dir.x * dir.y * (dir.x * dir.x - dir.y * dir.y);;
#endif
}

template <typename T>
auto eval_sh_4n3(const tvec3<T>& dir){
#ifndef USE_HARD_CODED
	return T(0.75) * std::sqrt(T(17.5) * invPI<T>) * dir.y * dir.z * (3 * dir.x * dir.x - dir.y * dir.y);
#else
	return T(-1.770131) * dir.y * dir.z * (3 * dir.x * dir.x - dir.y * dir.y);
#endif
}

template <typename T>
auto eval_sh_4n2(const tvec3<T>& dir){
#ifndef USE_HARD_CODED
	return T(0.75) * std::sqrt(5 * invPI<T>) * dir.x * dir.y * (7 * dir.z * dir.z - 1);
#else
	return T(0.946175) * dir.x * dir.y * (7 * dir.z * dir.z - 1);
#endif
}

template <typename T>
auto eval_sh_4n1(const tvec3<T>& dir){
#ifndef USE_HARD_CODED
	return T(0.75) * std::sqrt(T(2.5) * invPI<T>) * dir.y * dir.z * (7 * dir.z * dir.z - 3);
#else
	return T(-0.669047) * dir.y * dir.z * (7 * dir.z * dir.z - 3);
#endif
}

template <typename T>
auto eval_sh_40(const tvec3<T>& dir){
	T z2 = dir.z * dir.z;
#ifndef USE_HARD_CODED
	return T(3) / T(16) * std::sqrt(invPI<T>) * (35 * z2 * z2 - 30 * z2 + 3);
#else
	return T(0.105786) * (35 * z2 * z2 - 30 * z2 + 3);
#endif
}

template <typename T>
auto eval_sh_4p1(const tvec3<T>& dir){
#ifndef USE_HARD_CODED
	return T(0.75) * std::sqrt(T(2.5) * invPI<T>) * dir.x * dir.z * (7 * dir.z * dir.z - 3);
#else
	return T(-0.669047) * dir.x * dir.z * (7 * dir.z * dir.z - 3);
#endif
}

template <typename T>
auto eval_sh_4p2(const tvec3<T>& dir){
#ifndef USE_HARD_CODED
	return T(3) / T(8) * std::sqrt(5 * invPI<T>) * (dir.x * dir.x - dir.y * dir.y) * (7 * dir.z * dir.z - 1);
#else
	return T(0.473087) * (dir.x * dir.x - dir.y * dir.y) * (7 * dir.z * dir.z - 1);
#endif
}

template <typename T>
auto eval_sh_4p3(const tvec3<T>& dir){
#ifndef USE_HARD_CODED
	return T(0.75) * std::sqrt(T(17.5) * invPI<T>) * dir.x * dir.z * (dir.x * dir.x - 3 * dir.y * dir.y);
#else
	return T(-1.770131) * dir.x * dir.z * (dir.x * dir.x - 3 * dir.y * dir.y);
#endif
}

template <typename T>
auto eval_sh_4p4(const tvec3<T>& dir){
	T x2 = dir.x * dir.x;
	T y2 = dir.y * dir.y;
#ifndef USE_HARD_CODED
	return T(3) / T(16) * std::sqrt(35 * invPI<T>) * (x2 * (x2 - 3 * y2) - y2 * (3 * x2 - y2));
#else
	return T(0.625836) * (x2 * (x2 - 3 * y2) - y2 * (3 * x2 - y2));
#endif
}
#undef USE_HARD_CODED

template <typename T>
sh_expr_t<T>* sh_linear_table() noexcept{
	static sh_expr_t<T> table[] = {
		&eval_sh_00,
		&eval_sh_1n1,
		&eval_sh_10,
		&eval_sh_1p1,
		&eval_sh_2n2,
		&eval_sh_2n1,
		&eval_sh_20,
		&eval_sh_2p1,
		&eval_sh_2p2,
		&eval_sh_3n3,
		&eval_sh_3n2,
		&eval_sh_3n1,
		&eval_sh_30,
		&eval_sh_3p1,
		&eval_sh_3p2,
		&eval_sh_3p3,
		&eval_sh_4n4,
		&eval_sh_4n3,
		&eval_sh_4n2,
		&eval_sh_4n1,
		&eval_sh_40,
		&eval_sh_4p1,
		&eval_sh_4p2,
		&eval_sh_4p3,
		&eval_sh_4p4
	};
	static_assert(sizeof(table) == 25 * sizeof(sh_expr_t<T>),"");
	return table;
}

template <typename T, int L, int M>
auto eval_sh( const tvec3<T> &dir ) noexcept
{
	return sh_linear_table<T>()[get_linear_index(L,M)](dir);
}

template <typename T>
tvec3<T> to_vector(T phi,T theta){
	T r = std::sin(theta);
	return tvec3<T>(r * std::cos(phi), r * std::sin(phi), std::cos(theta));
}

template <typename T, int L, int M>
auto eval_sh(T phi, T theta) noexcept{
	return sh_linear_table<T>()[get_linear_index(L,M)](to_vector(phi,theta));
}

template <typename T>
void sh_rotate_degree_0(const tmat3_c<T>& rot,T* coefs){

}

template <typename T>
void sh_rotate_degree_1(const tmat3_c<T>& rot,T* coefs){

}

template <typename T>
void sh_rotate_degree_2(const tmat3_c<T>& rot,T* coefs){

}

template <typename T>
void sh_rotate_degree_3(const tmat3_c<T>& rot,T* coefs){

}

template <typename T>
void sh_rotate_degree_4(const tmat3_c<T>& rot,T* coefs){

}

template <typename T>
rotate_func_t<T>* sh_rot_table(){
	static rotate_func_t<T> table[] = {
		&sh_rotate_degree_0,
		&sh_rotate_degree_1,
		&sh_rotate_degree_2,
		&sh_rotate_degree_3,
		&sh_rotate_degree_4
	};
	return &table;
}

}// namespace detail

template <typename T, int L, int M>
auto eval_sh( const tvec3<T> &dir ) noexcept{
	return detail::eval_sh<T,L,M>(dir.normalized());
}

template <typename T, int L, int M>
auto eval_sh(T phi, T theta) noexcept{
	return detail::eval_sh<T,L,M>(phi,theta);
}

template <typename T>
sh_expr_t<T> *sh_linear_table() noexcept{
	return detail::sh_linear_table<T>();
}

template <typename T, int L>
tvec<T, 2 * L + 1> project_to_sh( const tvec3<T> &dir, T value ) noexcept{
	const auto table = detail::sh_linear_table<T>();
	const auto n = dir.normalized();
	tvec<T,2 * L + 1> ans;
	for(int i = 0, j = L * L; i < 2 * L + 1;)
		ans[i++] = table[j++](n);
	return ans;
}

template <typename T, int L>
auto project_func_to_sh( const sh_func_t<T> &func, int sample_count ){
	const auto table = detail::sh_linear_table<T>();
	const int sample_side = static_cast<int>(std::floor(std::sqrt(sample_count)));
	tvec<T, get_coef_count(L)>  ans(0);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> rng(0.0, 1.0);
	for(int i = 0; i < sample_side; ++i){
		for(int j = 0; j < sample_side; ++j){
			T u = (i + rng(gen)) / sample_side;
			T v = (j + rng(gen)) / sample_side;
			// uniform sample on sphere
			T phi  = 2 * PI<T> * u;
			T theta = std::acos(2 * v - 1.0);

			T fv = func(phi,theta);

			for(int l = 0; l <= L; ++l){
				for(int m = -l; m <= l; ++m){
					int index = get_linear_index(l,m);
					ans[index] += fv * table[index]( detail::to_vector(phi,theta));
				}
			}
		}
	}
	T weight = T(4) * PI<T> / (sample_side * sample_side);
	return ans.map([weight](T x){
		return x * weight;
	});
}

template <typename T>
auto project_func_to_sh(int degree, const sh_func_t<T> &func, int sample_count ){
    const auto table = detail::sh_linear_table<T>();
    const int sample_side = static_cast<int>(std::floor(std::sqrt(sample_count)));
    std::vector<T> ans(get_coef_count(degree));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> rng(0.0, 1.0);
    for(int i = 0; i < sample_side; ++i){
        for(int j = 0; j < sample_side; ++j){
            T u = (i + rng(gen)) / sample_side;
            T v = (j + rng(gen)) / sample_side;
            // uniform sample on sphere
            T phi  = 2 * PI<T> * u;
            T theta = std::acos(2 * v - 1.0);

            T fv = func(phi,theta);

            for(int l = 0; l <= degree; ++l){
                for(int m = -l; m <= l; ++m){
                    int index = get_linear_index(l,m);
                    ans[index] += fv * table[index]( detail::to_vector(phi,theta));
                }
            }
        }
    }
    T weight = T(4) * PI<T> / (sample_side * sample_side);
    for(auto& x : ans) x *= weight;
    return ans;
}

template <typename T, int DIM>
auto project_func_to_sh( int degree, const std::function<tvec<T,DIM>(T,T)> &func, int sample_count ){
    const auto table = detail::sh_linear_table<T>();
    const int sample_side = static_cast<int>(std::floor(std::sqrt(sample_count)));
    std::vector<tvec<T,DIM>> ans(get_coef_count(degree));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> rng(0.0, 1.0);
    for(int i = 0; i < sample_side; ++i){
        for(int j = 0; j < sample_side; ++j){
            T u = (i + rng(gen)) / sample_side;
            T v = (j + rng(gen)) / sample_side;
            // uniform sample on sphere
            T phi  = 2 * PI<T> * u;
            T theta = std::acos(2 * v - 1.0);//-1 ~ 1

            tvec<T,DIM> fv = func(phi,theta);

            for(int l = 0; l <= degree; ++l){
                for(int m = -l; m <= l; ++m){
                    int index = get_linear_index(l,m);
                    ans[index] += fv * table[index]( detail::to_vector(phi,theta));
                }
            }
        }
    }
    T weight = T(4) * PI<T> / (sample_side * sample_side);
    for(auto& x : ans) x *= weight;
    return ans;
}

template <typename T, int L>
void rotate_sh_coefs( const tmat3_c<T> &rot, T *coefs ) noexcept{
	detail::sh_rot_table<T>()[L](rot,coefs);
}

}// namespace sh
}// namespace wzz::math
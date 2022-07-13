#pragma once

#include <random>

namespace wzz::math {

    namespace sh {

        namespace detail {

            template<typename T>
            constexpr auto eval_sh_00(const tvec3<T> &dir) {
#ifdef NO_USE_HARD_CODED
                return T(0.5) * std::sqrt(invPI<T>);
#else
                return static_cast<T>(0.282095);
#endif
            }

            template<typename T>
            auto eval_sh_1n1(const tvec3<T> &dir) {
#ifdef NO_USE_HARD_CODED
                return std::sqrt(T(0.75) * invPI<T>) * dir.y;
#else
                return static_cast<T>(0.488603) * dir.y;
#endif
            }

            template<typename T>
            auto eval_sh_10(const tvec3<T> &dir) {
#ifdef NO_USE_HARD_CODED
                return std::sqrt(T(0.75) * invPI<T>) * dir.z;
#else
                return T(0.488603) * dir.z;
#endif
            }

            template<typename T>
            auto eval_sh_1p1(const tvec3<T> &dir) {
#ifdef NO_USE_HARD_CODED
                return std::sqrt(T(0.75) * invPI<T>) * dir.x;
#else
                return T(0.488603) * dir.x;
#endif
            }

            template<typename T>
            auto eval_sh_2n2(const tvec3<T> &dir) {
#ifdef NO_USE_HARD_CODED
                return  T(0.5) * std::sqrt(15 * invPI<T>) * dir.x * dir.y;
#else
                return T(1.092548) * dir.x * dir.y;
#endif
            }

            template<typename T>
            auto eval_sh_2n1(const tvec3<T> &dir) {
#ifdef NO_USE_HARD_CODED
                return  T(0.5) * std::sqrt(15 * invPI<T>) * dir.y * dir.z;
#else
                return T(1.092548) * dir.y * dir.z;
#endif
            }

            template<typename T>
            auto eval_sh_20(const tvec3<T> &dir) {
#ifdef NO_USE_HARD_CODED
                return T(0.25) * std::sqrt(5 * invPI<T>) * (-dir.x * dir.x - dir.y * dir.y + 2 * dir.z * dir.z);
#else
                return T(0.315392) * (-dir.x * dir.x - dir.y * dir.y + 2 * dir.z * dir.z);
#endif
            }

            template<typename T>
            auto eval_sh_2p1(const tvec3<T> &dir) {
#ifdef NO_USE_HARD_CODED
                return  T(0.5) * std::sqrt(15 * invPI<T>) * dir.x * dir.z;
#else
                return T(1.092548) * dir.x * dir.z;
#endif
            }

            template<typename T>
            auto eval_sh_2p2(const tvec3<T> &dir) {
#ifdef NO_USE_HARD_CODED
                return T(0.25) * std::sqrt(15 * invPI<T>) * (dir.x * dir.x - dir.y * dir.y);
#else
                return T(0.546274) * (dir.x * dir.x - dir.y * dir.y);
#endif
            }

            template<typename T>
            auto eval_sh_3n3(const tvec3<T> &dir) {
#ifdef NO_USE_HARD_CODED
                return T(0.25) * std::sqrt(T(17.5) * invPI<T>) * dir.y * (3 * dir.x * dir.x - dir.y * dir.y);
#else
                return T(0.590044) * dir.y * (3 * dir.x * dir.x - dir.y * dir.y);
#endif
            }

            template<typename T>
            auto eval_sh_3n2(const tvec3<T> &dir) {
#ifdef NO_USE_HARD_CODED
                return T(0.5) * std::sqrt(105 * invPI<T>) * dir.x * dir.y * dir.z;
#else
                return T(2.890611) * dir.x * dir.y * dir.z;
#endif
            }

            template<typename T>
            auto eval_sh_3n1(const tvec3<T> &dir) {
#ifdef NO_USE_HARD_CODED
                return T(0.25) * std::sqrt(T(10.5) * invPI<T>) * dir.y * (4 * dir.z * dir.z - dir.x * dir.x - dir.y * dir.y);
#else
                return T(0.457046) * dir.y * (4 * dir.z * dir.z - dir.x * dir.x - dir.y * dir.y);
#endif
            }

            template<typename T>
            auto eval_sh_30(const tvec3<T> &dir) {
#ifdef NO_USE_HARD_CODED
                return T(0.25) * std::sqrt(7 * invPI<T>) * dir.z * (2 * dir.z * dir.z - 3 * dir.x * dir.x - 3 * dir.y * dir.y);
#else
                return T(0.373176) * dir.z * (2 * dir.z * dir.z - 3 * dir.x * dir.x - 3 * dir.y * dir.y);
#endif
            }

            template<typename T>
            auto eval_sh_3p1(const tvec3<T> &dir) {
#ifdef NO_USE_HARD_CODED
                return T(0.25) * std::sqrt(T(10.5) * invPI<T>) * dir.x * (4 * dir.z * dir.z - dir.x * dir.x - dir.y * dir.y);
#else
                return T(0.457046) * dir.x * (4 * dir.z * dir.z - dir.x * dir.x - dir.y * dir.y);
#endif
            }

            template<typename T>
            auto eval_sh_3p2(const tvec3<T> &dir) {
#ifdef NO_USE_HARD_CODED
                return T(0.25) * std::sqrt(105 * invPI<T>) * dir.z * (dir.x * dir.x - dir.y * dir.y);
#else
                return T(1.445306) * dir.z * (dir.x * dir.x - dir.y * dir.y);
#endif
            }

            template<typename T>
            auto eval_sh_3p3(const tvec3<T> &dir) {
#ifdef NO_USE_HARD_CODED
                return T(0.25) * std::sqrt(T(17.5) * invPI<T>) * dir.x * (dir.x * dir.x - 3 * dir.y * dir.y);
#else
                return T(0.590044) * dir.x * (dir.x * dir.x - 3 * dir.y * dir.y);;
#endif
            }

            template<typename T>
            auto eval_sh_4n4(const tvec3<T> &dir) {
#ifdef NO_USE_HARD_CODED
                return T(0.75) * std::sqrt(35 * invPI<T>) * dir.x * dir.y * (dir.x * dir.x - dir.y * dir.y);
#else
                return T(2.503343) * dir.x * dir.y * (dir.x * dir.x - dir.y * dir.y);;
#endif
            }

            template<typename T>
            auto eval_sh_4n3(const tvec3<T> &dir) {
#ifdef NO_USE_HARD_CODED
                return T(0.75) * std::sqrt(T(17.5) * invPI<T>) * dir.y * dir.z * (3 * dir.x * dir.x - dir.y * dir.y);
#else
                return T(1.770131) * dir.y * dir.z * (3 * dir.x * dir.x - dir.y * dir.y);
#endif
            }

            template<typename T>
            auto eval_sh_4n2(const tvec3<T> &dir) {
#ifdef NO_USE_HARD_CODED
                return T(0.75) * std::sqrt(5 * invPI<T>) * dir.x * dir.y * (7 * dir.z * dir.z - 1);
#else
                return T(0.946175) * dir.x * dir.y * (7 * dir.z * dir.z - 1);
#endif
            }

            template<typename T>
            auto eval_sh_4n1(const tvec3<T> &dir) {
#ifdef NO_USE_HARD_CODED
                return T(0.75) * std::sqrt(T(2.5) * invPI<T>) * dir.y * dir.z * (7 * dir.z * dir.z - 3);
#else
                return T(0.669047) * dir.y * dir.z * (7 * dir.z * dir.z - 3);
#endif
            }

            template<typename T>
            auto eval_sh_40(const tvec3<T> &dir) {
                T z2 = dir.z * dir.z;
#ifdef NO_USE_HARD_CODED
                return T(3) / T(16) * std::sqrt(invPI<T>) * (35 * z2 * z2 - 30 * z2 + 3);
#else
                return T(0.105786) * (35 * z2 * z2 - 30 * z2 + 3);
#endif
            }

            template<typename T>
            auto eval_sh_4p1(const tvec3<T> &dir) {
#ifdef NO_USE_HARD_CODED
                return T(0.75) * std::sqrt(T(2.5) * invPI<T>) * dir.x * dir.z * (7 * dir.z * dir.z - 3);
#else
                return T(0.669047) * dir.x * dir.z * (7 * dir.z * dir.z - 3);
#endif
            }

            template<typename T>
            auto eval_sh_4p2(const tvec3<T> &dir) {
#ifdef NO_USE_HARD_CODED
                return T(3) / T(8) * std::sqrt(5 * invPI<T>) * (dir.x * dir.x - dir.y * dir.y) * (7 * dir.z * dir.z - 1);
#else
                return T(0.473087) * (dir.x * dir.x - dir.y * dir.y) * (7 * dir.z * dir.z - 1);
#endif
            }

            template<typename T>
            auto eval_sh_4p3(const tvec3<T> &dir) {
#ifdef NO_USE_HARD_CODED
                return T(0.75) * std::sqrt(T(17.5) * invPI<T>) * dir.x * dir.z * (dir.x * dir.x - 3 * dir.y * dir.y);
#else
                return T(1.770131) * dir.x * dir.z * (dir.x * dir.x - 3 * dir.y * dir.y);
#endif
            }

            template<typename T>
            auto eval_sh_4p4(const tvec3<T> &dir) {
                T x2 = dir.x * dir.x;
                T y2 = dir.y * dir.y;
#ifdef NO_USE_HARD_CODED
                return T(3) / T(16) * std::sqrt(35 * invPI<T>) * (x2 * (x2 - 3 * y2) - y2 * (3 * x2 - y2));
#else
                return T(0.625836) * (x2 * (x2 - 3 * y2) - y2 * (3 * x2 - y2));
#endif
            }

#undef USE_HARD_CODED

            template<typename T>
            sh_expr_t<T> *sh_linear_table() noexcept {
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
                static_assert(sizeof(table) == 25 * sizeof(sh_expr_t<T>), "");
                return table;
            }

            template<typename T, int L, int M>
            auto eval_sh(const tvec3<T> &dir) noexcept {
                return sh_linear_table<T>()[get_linear_index(L, M)](dir);
            }

            template<typename T>
            tvec3<T> to_vector(T phi, T theta) {
                T r = std::sin(theta);
                return tvec3<T>(r * std::cos(phi), r * std::sin(phi), std::cos(theta));
            }

            template<typename T, int L, int M>
            auto eval_sh(T phi, T theta) noexcept {
                return sh_linear_table<T>()[get_linear_index(L, M)](to_vector(phi, theta));
            }

            template<typename T>
            void sh_rotate_degree_0(const tmat3_c<T> &rot, T *coefs) {
                //none operation
            }

            // ref/copy https://github.com/AirGuanZ/agz-utils/blob/master/include/agz-utils/math/impl/spherical_harmonics.inl
            // see https://zhuanlan.zhihu.com/p/51267461
            template<typename T>
            void sh_rotate_degree_1(const tmat3_c<T> &rot, T *coefs) {
                static const T INV_C = std::sqrt(4 * PI<T> / 3);

                static const tmat3_c<T> INV_A(0, 0, INV_C,
                                              INV_C, 0, 0,
                                              0, INV_C, 0);

                // 由于N_i被选取为各轴上的方向向量，故M * Ni就是M的第i列

                const auto PMN0 = sh::project_to_sh<1>(rot.get_col(0));
                const auto PMN1 = sh::project_to_sh<1>(rot.get_col(1));
                const auto PMN2 = sh::project_to_sh<1>(rot.get_col(2));
                const tmat3_c<T> S = tmat3_c<T>::from_cols(
                        {PMN0[0], PMN0[1], PMN0[2]},
                        {PMN1[0], PMN1[1], PMN1[2]},
                        {PMN2[0], PMN2[1], PMN2[2]});

                tvec3<T> x(coefs[0], coefs[1], coefs[2]);
                x = S * (INV_A * x);

                coefs[0] = x[0];
                coefs[1] = x[1];
                coefs[2] = x[2];
            }

            template<typename T>
            void sh_rotate_degree_2(const tmat3_c<T> &rot, T *coefs) {
                constexpr T K = T(0.7071067811865475);
                static const tvec3<T> N2(K, K, T(0));
                static const tvec3<T> N3(K, T(0), K);
                static const tvec3<T> N4(T(0), K, K);

                constexpr T K0 = T(0.91529123286551084);
                constexpr T K1 = T(1.83058246573102168);
                constexpr T K2 = T(1.5853309190550713);

                // 计算A^{-1}x

                tvec<T, 5> invAx(K0 * (coefs[1] - coefs[3]) + K1 * coefs[4],
                                 K0 * (coefs[0] - coefs[3] + coefs[4]) + K2 * coefs[2],
                                 K1 * coefs[0],
                                 K1 * coefs[3],
                                 K1 * coefs[1]);

                // 构造S

                const auto PMN0 = sh::project_to_sh<2>(rot.get_col(0));
                const auto PMN1 = sh::project_to_sh<2>(rot.get_col(2));
                const auto PMN2 = sh::project_to_sh<2>(rot * N2);
                const auto PMN3 = sh::project_to_sh<2>(rot * N3);
                const auto PMN4 = sh::project_to_sh<2>(rot * N4);

                // 计算S(A^{-1}x)

                coefs[0] = PMN0[0] * invAx[0] + PMN1[0] * invAx[1] + PMN2[0] * invAx[2] + PMN3[0] * invAx[3] +
                           PMN4[0] * invAx[4];
                coefs[1] = PMN0[1] * invAx[0] + PMN1[1] * invAx[1] + PMN2[1] * invAx[2] + PMN3[1] * invAx[3] +
                           PMN4[1] * invAx[4];
                coefs[2] = PMN0[2] * invAx[0] + PMN1[2] * invAx[1] + PMN2[2] * invAx[2] + PMN3[2] * invAx[3] +
                           PMN4[2] * invAx[4];
                coefs[3] = PMN0[3] * invAx[0] + PMN1[3] * invAx[1] + PMN2[3] * invAx[2] + PMN3[3] * invAx[3] +
                           PMN4[3] * invAx[4];
                coefs[4] = PMN0[4] * invAx[0] + PMN1[4] * invAx[1] + PMN2[4] * invAx[2] + PMN3[4] * invAx[3] +
                           PMN4[4] * invAx[4];

            }

            template<typename T>
            void sh_rotate_degree_3(const tmat3_c<T> &rot, T *coefs) {
                static const tvec3<T> N[7] = {
                        tvec3<T>(T(1), T(0), T(0)).normalized(),
                        tvec3<T>(T(0), T(1), T(0)).normalized(),
                        tvec3<T>(T(0.3), T(0), T(1)).normalized(),
                        tvec3<T>(T(0), T(1), T(1)).normalized(),
                        tvec3<T>(T(1), T(0), T(1)).normalized(),
                        tvec3<T>(T(1), T(1), T(0.78)).normalized(),
                        tvec3<T>(T(1), T(1), T(1)).normalized()
                };

                static const T invA[7][7] = {
                        {T(0.707711955885399),   T(0.643852929494021),  T(-0.913652206352009),  T(-0.093033334712756), T(0.328680372803511),   T(-1.131667680791894),  T(1.949384763080401)},
                        {T(-1.114187338255984),  T(0.643852929494021),  T(-0.749554866243252),  T(-0.093033334712757), T(0.164583032694754),   T(-0.232204002745663),  T(0.127485468939019)},
                        {T(2.296023687102124),   T(0),                  T(-2.964153834214758),  T(0),                  T(2.964153834214758),   T(-3.749390980495911),  T(2.296023687102124)},
                        {T(2.392306681179504),   T(-1.099424142052695), T(-3.088454645076318),  T(-2.129025696294232), T(3.766408103751610),   T(-5.313883353254694),  T(2.917447172170129)},
                        {T(1.878707739441422),   T(-1.099424142052695), T(-2.425401262415870),  T(-2.129025696294233), T(3.103354721091161),   T(-2.518204820606409),  T(2.403848230432046)},
                        {T(13.656934981397061),  T(-4.181565269348606), T(-17.631027247729438), T(-8.097566324633245), T(14.325209638780166),  T(-20.210898801851609), T(11.096259672385109)},
                        {T(-13.139185354460187), T(5.820633765367933),  T(16.962615353518899),  T(7.790578559853934),  T(-13.782124974734103), T(19.444681101542464),  T(-10.675588100498899)},
                };

                tvec<T, 7> invAx(T(0));
                for (int i = 0; i != 7; ++i) {
                    for (int j = 0; j != 7; ++j)
                        invAx[i] += invA[i][j] * coefs[j];
                }

                tvec<T, 7> PMN[7];
                for (int i = 0; i < 7; ++i)
                    PMN[i] = sh::project_to_sh<3>(rot * N[i]);

                for (int i = 0; i < 7; ++i) {
                    coefs[i] = T(0);
                    for (int j = 0; j < 7; ++j)
                        coefs[i] += PMN[j][i] * invAx[j];
                }
            }

            template<typename T>
            void sh_rotate_degree_4(const tmat3_c<T> &rot, T *coefs) {
                static const tvec3<T> N[9] = {
                        tvec3<T>(T(1), T(0), T(0)).normalized(),
                        tvec3<T>(T(0), T(1), T(0)).normalized(),
                        tvec3<T>(T(0.3), T(0), T(1)).normalized(),
                        tvec3<T>(T(0), T(1), T(1)).normalized(),
                        tvec3<T>(T(1), T(0), T(1)).normalized(),
                        tvec3<T>(T(1), T(0.54), T(0.78)).normalized(),
                        tvec3<T>(T(1), T(1), T(0.78)).normalized(),
                        tvec3<T>(T(0.31), T(1), T(0.78)).normalized(),
                        tvec3<T>(T(1), T(1), T(1)).normalized()
                };

                static const T invA[9][9] = {
                        {T(-1.948206991589258), T(1.912687049138671),  T(-0.763091021186035), T(-0.286837642392582), T(-0.341264679278342), T(0.594477634079894),  T(-1.056887279361603), T(0.578857155270682),  T(0.971984464556520)},
                        {T(2.171192074917378),  T(-0.142084581369102), T(-1.577618721617938), T(0.828536347413562),  T(-0.705532540822805), T(0.382031320127708),  T(1.056887279361603),  T(-2.513802449733083), T(1.156701984383617)},
                        {T(2.053952330860290),  T(-0.094158653118148), T(-0.750956907863241), T(-1.098731135021785), T(-0.335838138831051), T(1.931188736063331),  T(0),                  T(-1.051043414216722), T(0.170301019159901)},
                        {T(3.993132334888566),  T(1.179414191911931),  T(-4.808985771815311), T(1.266884703225481),  T(-3.095952538204609), T(2.811562290853012),  T(0),                  T(-4.022967497037739), T(1.569934476060706)},
                        {T(-1.543780567538975), T(1.894449743774703),  T(-2.499709102566265), T(-0.207318037527907), T(-2.063212615945576), T(1.725864595116423),  T(0),                  T(-0.365404044003703), T(1.046239752465574)},
                        {T(3.435134010827782),  T(-2.932684025967419), T(4.231264528651311),  T(-2.972023260715974), T(1.892279023369589),  T(-1.718456688280952), T(0),                  T(2.458880397035034),  T(-0.959560600640598)},
                        {T(3.689266412234284),  T(1.985158283498190),  T(-7.403078714786565), T(-3.123392326177335), T(-3.310757449808909), T(3.006635497533013),  T(0),                  T(-4.302091019418769), T(1.678860447048080)},
                        {T(-0.367659806642012), T(-3.222124483746851), T(4.648868038376401),  T(-3.265346293642776), T(2.079036990447149),  T(-1.888059306949047), T(0),                  T(2.701558933638689),  T(-1.054264174928627)},
                        {T(-4.515212732000947), T(3.220651333447782),  T(0.208527587656698),  T(6.066568738154828),  T(-0.970215938306426), T(0.881093140952614),  T(0),                  T(-1.260725782049042), T(0.491989276959057)},
                };

                tvec<T, 9> invAx(T(0));
                for (int i = 0; i != 9; ++i) {
                    for (int j = 0; j != 9; ++j)
                        invAx[i] += invA[i][j] * coefs[j];
                }

                tvec<T, 9> PMN[9];
                for (int i = 0; i < 9; ++i)
                    PMN[i] = sh::project_to_sh<4>(rot * N[i]);

                for (int i = 0; i < 9; ++i) {
                    coefs[i] = T(0);
                    for (int j = 0; j < 9; ++j)
                        coefs[i] += PMN[j][i] * invAx[j];
                }
            }

            template<typename T>
            rotate_func_t<T> *sh_rot_table() {
                static rotate_func_t<T> table[] = {
                        &sh_rotate_degree_0,
                        &sh_rotate_degree_1,
                        &sh_rotate_degree_2,
                        &sh_rotate_degree_3,
                        &sh_rotate_degree_4
                };
                return table;
            }

        }// namespace detail

        template<typename T, int L, int M>
        auto eval_sh(const tvec3<T> &dir) noexcept {
            return detail::eval_sh<T, L, M>(dir.normalized());
        }

        template<typename T, int L, int M>
        auto eval_sh(T phi, T theta) noexcept {
            return detail::eval_sh<T, L, M>(phi, theta);
        }

        template<typename T>
        sh_expr_t<T> *sh_linear_table() noexcept {
            return detail::sh_linear_table<T>();
        }

        template<int L, typename T>
        tvec<T, 2 * L + 1> project_to_sh(const tvec3<T> &dir, T value) noexcept {
            const auto table = detail::sh_linear_table<T>();
            const auto n = dir.normalized();
            tvec<T, 2 * L + 1> ans;
            for (int i = 0, j = L * L; i < 2 * L + 1;)
                ans[i++] = value * table[j++](n);
            return ans;
        }

        template<typename T, int L>
        tvec<T, get_coef_count(L)> project_func_to_sh(const sh_func_t<T> &func, int sample_count) {
            const auto table = detail::sh_linear_table<T>();
            const int sample_side = static_cast<int>(std::floor(std::sqrt(sample_count)));
            tvec<T, get_coef_count(L)> ans(0);

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> rng(0.0, 1.0);

            for (int i = 0; i < sample_side; ++i) {
                for (int j = 0; j < sample_side; ++j) {
                    T u = (i + rng(gen)) / sample_side;
                    T v = (j + rng(gen)) / sample_side;
                    // uniform sample on sphere
                    T phi = 2 * PI<T> * u;
                    T theta = std::acos(2 * v - 1.0);

                    T fv = func(phi, theta);

                    for (int l = 0; l <= L; ++l) {
                        for (int m = -l; m <= l; ++m) {
                            int index = get_linear_index(l, m);
                            ans[index] += fv * table[index](detail::to_vector(phi, theta));
                        }
                    }
                }
            }
            T weight = T(4) * PI<T> / (sample_side * sample_side);
            return ans.map([weight](T x) {
                return x * weight;
            });
        }

        template<typename T>
        std::vector<T> project_func_to_sh(int degree, const sh_func_t<T> &func, int sample_count) {
            const auto table = detail::sh_linear_table<T>();
            const int sample_side = static_cast<int>(std::floor(std::sqrt(sample_count)));
            std::vector<T> ans(get_coef_count(degree));
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> rng(0.0, 1.0);
            for (int i = 0; i < sample_side; ++i) {
                for (int j = 0; j < sample_side; ++j) {
                    T u = (i + rng(gen)) / sample_side;
                    T v = (j + rng(gen)) / sample_side;
                    // uniform sample on sphere
                    T phi = 2 * PI<T> * u;
                    T theta = std::acos(2 * v - 1.0);

                    T fv = func(phi, theta);

                    for (int l = 0; l <= degree; ++l) {
                        for (int m = -l; m <= l; ++m) {
                            int index = get_linear_index(l, m);
                            ans[index] += fv * table[index](detail::to_vector(phi, theta));
                        }
                    }
                }
            }
            T weight = T(4) * PI<T> / (sample_side * sample_side);
            for (auto &x: ans) x *= weight;
            return ans;
        }

        template<typename T, int DIM>
        std::vector<tvec<T,DIM>> project_func_to_sh(int degree, const std::function<tvec<T, DIM>(T, T)> &func, int sample_count) {
            const auto table = detail::sh_linear_table<T>();
            const int sample_side = static_cast<int>(std::floor(std::sqrt(sample_count)));
            std::vector<tvec<T, DIM>> ans(get_coef_count(degree));
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> rng(0.0, 1.0);
            for (int i = 0; i < sample_side; ++i) {
                for (int j = 0; j < sample_side; ++j) {
                    T u = (i + rng(gen)) / sample_side;
                    T v = (j + rng(gen)) / sample_side;
                    // uniform sample on sphere
                    T phi = 2 * PI<T> * u;
                    T theta = std::acos(2 * v - 1.0);//-1 ~ 1

                    tvec<T, DIM> fv = func(phi, theta);

                    for (int l = 0; l <= degree; ++l) {
                        for (int m = -l; m <= l; ++m) {
                            int index = get_linear_index(l, m);
                            ans[index] += fv * table[index](detail::to_vector(phi, theta));
                        }
                    }
                }
            }
            T weight = T(4) * PI<T> / (sample_side * sample_side);
            for (auto &x: ans) x *= weight;
            return ans;
        }

        template<typename T>
        void rotate_sh_coefs(const tmat3_c<T> &rot, int degree, T *coefs) noexcept {
            detail::sh_rot_table<T>()[degree](rot, coefs);
        }

    }// namespace sh
}// namespace wzz::math
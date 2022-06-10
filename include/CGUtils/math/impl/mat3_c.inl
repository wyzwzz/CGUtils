#pragma once
WZZ_MATH_BEGIN

template <typename T>
tmat3_c<T>::tmat3_c() noexcept
:tmat3_c(identity())
{

}

template <typename T>
tmat3_c<T>::tmat3_c(uninitialized_t) noexcept
:data{col_t(uninitialized_t{}),col_t(uninitialized_t{}),col_t(uninitialized_t{})}
{

}

template <typename T>
tmat3_c<T>::tmat3_c(T m00, T m01, T m02,
                    T m10, T m11, T m12,
                    T m20, T m21, T m22) noexcept
:data{ {m00,m10,m20},
       {m01,m11,m21},
       {m02,m12,m22} }
{

}

template <typename T>
tmat3_c<T> tmat3_c<T>::from_rows(const row_t &r0, const row_t &r1, const row_t &r2) noexcept
{
    return self_t(r0.x,r0.y,r0.z,
                  r1.x,r1.y,r1.z,
                  r2.x,r2.y,r2.z);
}

template <typename T>
tmat3_c<T> tmat3_c<T>::from_cols(const col_t &c0, const col_t &c1, const col_t &c2) noexcept
{
    return self_t(c0.x,c1.x,c2.x,
                  c0.y,c1.y,c2.y,
                  c0.z,c1.z,c2.z);
}

template <typename T>
tmat3_c<T> tmat3_c<T>::diag(T val) noexcept
{
    return self_t(val, 0   , 0 ,
                   0 , val , 0 ,
                   0 , 0   , val);
}

template <typename T>
tmat3_c<T> tmat3_c<T>::all(T val) noexcept
{
    return self_t(val, val, val,
                  val, val, val,
                  val, val, val);
}

template <typename T>
tmat3_c<T> tmat3_c<T>::identity() noexcept
{
    return diag(1);
}

template<typename T>
typename tmat3_c<T>::col_t& tmat3_c<T>::operator[](size_t idx) noexcept
{
    return data[idx];
}

template<typename T>
const typename tmat3_c<T>::col_t& tmat3_c<T>::operator[](
    size_t idx) const noexcept
{
    return data[idx];
}

template<typename T>
T &tmat3_c<T>::operator()(size_t row, size_t col) noexcept
{
    return data[col][row];
}

template<typename T>
const T &tmat3_c<T>::operator()(size_t row, size_t col) const noexcept
{
    return data[col][row];
}

template<typename T>
const tvec3<T>& tmat3_c<T>::get_col(size_t idx) const noexcept
{
    return data[idx];
}

template<typename T>
tvec3<T> tmat3_c<T>::get_row(size_t idx) const noexcept
{
    return tvec3<T>(data[0][idx], data[1][idx], data[2][idx]);
}

template<typename T>
bool tmat3_c<T>::operator==(const self_t &rhs) const noexcept
{
    return data[0] == rhs.data[0] &&
           data[1] == rhs.data[1] &&
           data[2] == rhs.data[2];
}

template<typename T>
bool tmat3_c<T>::operator!=(const self_t &rhs) const noexcept
{
    return !(*this == rhs);
}

template<typename T>
typename tmat3_c<T>::self_t &tmat3_c<T>::operator+=(
    const self_t &rhs) noexcept
{
    return *this = *this + rhs;
}

template<typename T>
typename tmat3_c<T>::self_t &tmat3_c<T>::operator-=(
    const self_t &rhs) noexcept
{
    return *this = *this - rhs;
}

template<typename T>
typename tmat3_c<T>::self_t &tmat3_c<T>::operator*=(
    const self_t &rhs) noexcept
{
    return *this = *this * rhs;
}

template<typename T>
typename tmat3_c<T>::self_t &tmat3_c<T>::operator*=(T rhs) noexcept
{
    return *this = *this * rhs;
}

template<typename T>
typename tmat3_c<T>::self_t &tmat3_c<T>::operator/=(T rhs) noexcept
{
    return *this = *this / rhs;
}

template<typename T>
tmat3_c<T> operator+(
    const tmat3_c<T> &lhs, const tmat3_c<T> &rhs) noexcept
{
    return tmat3_c<T>::from_cols(lhs[0] + rhs[0],
                                 lhs[1] + rhs[1],
                                 lhs[2] + rhs[2]);
}

template<typename T>
tmat3_c<T> operator-(
    const tmat3_c<T> &lhs, const tmat3_c<T> &rhs) noexcept
{
    return tmat3_c<T>::from_cols(lhs[0] - rhs[0],
                                 lhs[1] - rhs[1],
                                 lhs[2] - rhs[2]);
}

template<typename T>
tmat3_c<T> operator*(
    const tmat3_c<T> &lhs, const tmat3_c<T> &rhs) noexcept
{
    tmat3_c<T> ret(uninitialized_t{});
    for(int c = 0; c != 3; c++)
    {
        for(int r = 0; r != 3; r++)
            ret(r, c) = dot(lhs.get_row(r), rhs.get_col(c));
    }
    return ret;
}

template<typename T>
tvec3<T> operator*(
    const tmat3_c<T> &lhs, const tvec3<T> &rhs) noexcept
{
    return tvec3<T>(dot(lhs.get_row(0), rhs),
                    dot(lhs.get_row(1), rhs),
                    dot(lhs.get_row(2), rhs));
}

template<typename T>
tvec3<T> operator*(
    const tvec3<T> &lhs, const tmat3_c<T> &rhs) noexcept
{
    return tvec3<T>(dot(lhs, rhs.get_col(0)),
                    dot(lhs, rhs.get_col(1)),
                    dot(lhs, rhs.get_col(2)));
}

template<typename T>
tmat3_c<T> operator*(const tmat3_c<T> &lhs, T rhs) noexcept
{
    return tmat3_c<T>::from_cols(lhs[0] * rhs, lhs[1] * rhs, lhs[2] * rhs);
}

template<typename T>
tmat3_c<T> operator*(T lhs, const tmat3_c<T> &rhs) noexcept
{
    return rhs * lhs;
}

template<typename T>
tmat3_c<T> operator/(const tmat3_c<T> &lhs, T rhs) noexcept
{
    return tmat3_c<T>::from_cols(lhs[0] / rhs, lhs[1] / rhs, lhs[2] / rhs);
}

WZZ_MATH_END
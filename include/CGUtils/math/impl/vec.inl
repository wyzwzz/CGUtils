#pragma once

WYZ_MATH_BEGIN

template <typename T, int D>
tvec<T, D>::tvec() noexcept
:tvec(T(0))
{

}

template <typename T, int D>
tvec<T,D>::tvec(T val) noexcept
{
    for(int i =0;i<D;i++)
        data[i] = val;
}

template <typename T, int D>
tvec<T,D>::tvec(uninitialized_t) noexcept
{

}

template <typename T, int D>
template <typename... As,typename >
tvec<T, D>::tvec(As&&... args) noexcept
: data{T(std::forward<As>(args))...}
{

}

template <typename T, int D>
T& tvec<T,D>::operator[](int idx) noexcept
{
    return data[idx];
}

template <typename T, int D>
const T& tvec<T,D>::operator[](int idx) const noexcept
{
    return data[idx];
}

WYZ_MATH_END
#pragma once


namespace wzz::math{

template <typename T>
tcolor3<T>::tcolor3()
:tcolor3(T())
{
}
template <typename T>
tcolor3<T>::tcolor3( T v ) noexcept
:tcolor3(v,v,v)
{
}
template <typename T>
tcolor3<T>::tcolor3( T r, T g, T b ) noexcept
:r(r),g(g),b(b)
{

}

}
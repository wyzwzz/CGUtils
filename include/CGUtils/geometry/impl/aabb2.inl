#pragma once

WZZ_MATH_BEGIN

template <typename T>
taabb2<T>::taabb2() noexcept
{

}

template <typename T>
taabb2<T>::taabb2(uninitialized_t) noexcept
:low(uninitialized_t{}),high(uninitialized_t{})
{

}

template <typename T>
taabb2<T>::taabb2(const tvec2<T> &low, const tvec2<T> &high) noexcept
:low(low),high(high)
{

}

template <typename T>
bool taabb2<T>::is_valid() const noexcept
{
    return low.x <= high.x && low.y <= high.y;
}

template <typename T>
bool taabb2<T>::intersect(const self_t &other) const noexcept
{
    return (*this | other).is_valid();
}

template <typename T>
bool taabb2<T>::contains(const self_t &other) const noexcept
{
    return other.within(*this);
}

template <typename T>
bool taabb2<T>::contains(const tvec2<T> &other) const noexcept
{
    return low.x <= other.x && low.y <= other.y
    && high.x >= other.x && high.y >= other.y;
}

template <typename T>
bool taabb2<T>::within(const self_t &other) const noexcept
{
    return low.x <= other.low.x && low.y <= other.low.y
    && high.x >= other.high.x && high.y >= other.high.y;
}

template <typename T>
taabb2<T>& taabb2<T>::operator|=(const taabb2<T>& rhs) noexcept{
    return *this = *this | rhs;
}

template <typename T>
taabb2<T>& taabb2<T>::operator|=(const tvec2<T>& rhs) noexcept{
    return *this = *this | rhs;
}

template <typename T>
taabb2<T>& taabb2<T>::operator&=(const taabb2<T> &rhs) noexcept
{
    return *this = *this & rhs;
}

template <typename T>
taabb2<T>& taabb2<T>::operator*=(const tvec2<T> &rhs) noexcept
{
    low *= rhs;
    high *= rhs;
    return *this;
}

template <typename T>
taabb2<T> operator|(const taabb2<T>& a,const taabb2<T>& b){
    return taabb2<T>(
        tvec2<T>((std::min)(a.low.x,b.low.x),(std::min)(a.low.y,b.low.y)),
        tvec2<T>((std::max)(a.high.x,b.high.x),(std::max)(a.high.y,b.high.y))
    );
}

template <typename T>
taabb2<T> operator|(const taabb2<T>& a,const tvec2<T>& b){
    return taabb2<T>(
        tvec2<T>((std::min)(a.low.x,b.x),(std::min)(a.low.y,b.y)),
        tvec2<T>((std::max)(a.high.x,b.x),(std::max)(a.high.y,b.y))
    );
}

template <typename T>
taabb2<T> operator&(const taabb2<T>& a,const taabb2<T>& b){
    return taabb2<T>(
        tvec2<T>((std::max)(a.low.x,b.low.x),(std::max)(a.low.y,b.low.y)),
        tvec2<T>((std::min)(a.high.x,b.high.x),(std::min)(a.high.y,b.high.y))
    );
}

template <typename T>
taabb2<T> operator*(const taabb2<T>& a,const tvec2<T>& b){
    return taabb2<T>(
        a.low * b,
        a.high * b
        );
}

WZZ_MATH_END
//
// Created by wyz on 2022/6/10.
//

#pragma once

namespace wzz::math{

inline simd_float3_t::simd_float3_t() noexcept
:simd_float3_t(0)
{}

inline simd_float3_t::simd_float3_t(float v) noexcept
: simd_float3_t(_mm_set_ps1(v))
{}
//__m128 _mm_set_ps (float e3, float e2, float e1, float e0)
//e0[0:31] == r == x
inline simd_float3_t::simd_float3_t(float x, float y, float z) noexcept
: simd_float3_t(_mm_set_ps(z,z,y,x))
{}

inline simd_float3_t::simd_float3_t(const __m128 &m128) noexcept
:m128(m128)
{}

inline simd_float3_t::simd_float3_t(const simd_float3_t &s) noexcept
:m128(s.m128)
{}

inline simd_float3_t& simd_float3_t::operator=(const simd_float3_t& s) noexcept{
    m128 = s.m128;
    return *this;
}



inline simd_float3_t simd_float3_t::operator+(const simd_float3_t& s) noexcept {
    return simd_float3_t(_mm_add_ps(m128,s.m128));
}


}
//
// Created by wyz on 2022/6/10.
//

#pragma once

#include <emmintrin.h>

namespace wzz::math{

    class alignas(16) simd_float3_t{
      public:

        union {
            __m128 m128;
            struct{
                float r, g, b, pad_a;
            };
            struct{
                float x, y, z, pad_w;
            };
        };

        simd_float3_t() noexcept;

        simd_float3_t(float x,float y,float z) noexcept;

        explicit simd_float3_t(float v) noexcept;

        explicit simd_float3_t(const __m128& m128) noexcept;

        simd_float3_t(const simd_float3_t&) noexcept;

        simd_float3_t& operator=(const simd_float3_t&) noexcept;

        //util functions
        bool is_zero() const noexcept;

        bool is_black() const noexcept;

        bool is_finite() const noexcept;

        bool not_negative() const noexcept;

        float length() const noexcept;

        float length_squared() const noexcept;

        simd_float3_t normalized() const noexcept;

        simd_float3_t& normalizing() noexcept;

        float lum() const noexcept;

        //operators
        bool operator!() const noexcept;

        float& operator[](int index) noexcept;
        const float& operator[](int index) const noexcept;

        bool operator==(const simd_float3_t& s) const noexcept;
        bool operator!=(const simd_float3_t& s) const noexcept;

        simd_float3_t& operator+=(float s) noexcept;
        simd_float3_t& operator-=(float s) noexcept;
        simd_float3_t& operator*=(float s) noexcept;
        simd_float3_t& operator/=(float s) noexcept;

        simd_float3_t& operator+=(const simd_float3_t& s) noexcept;
        simd_float3_t& operator-=(const simd_float3_t& s) noexcept;
        simd_float3_t& operator*=(const simd_float3_t& s) noexcept;
        simd_float3_t& operator/=(const simd_float3_t& s) noexcept;


        simd_float3_t operator+(float s) noexcept;
        simd_float3_t operator-(float s) noexcept;
        simd_float3_t operator*(float s) noexcept;
        simd_float3_t operator/(float s) noexcept;

        simd_float3_t operator+(const simd_float3_t& s) noexcept;
        simd_float3_t operator-(const simd_float3_t& s) noexcept;
        simd_float3_t operator*(const simd_float3_t& s) noexcept;
        simd_float3_t operator/(const simd_float3_t& s) noexcept;

    };
    static_assert(sizeof(simd_float3_t) == 16,"simd_float3_t's size not equal to 16");

    simd_float3_t operator+(float lhs,const simd_float3_t& rhs);
    simd_float3_t operator-(float lhs,const simd_float3_t& rhs);
    simd_float3_t operator*(float lhs,const simd_float3_t& rhs);

}

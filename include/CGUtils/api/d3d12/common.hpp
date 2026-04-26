#pragma once

#include <d3d12.h>
#include <system_error>

#include <CGUtils/math.hpp>

#define WZZ_D3D12_BEGIN namespace wzz::d3d12 {
#define WZZ_D3D12_END }

WZZ_D3D12_BEGIN

using Float2 = math::vec2f;
using Float3 = math::vec3f;
using Float4 = math::vec4f;

using Double2 = math::vec2d;
using Double3 = math::vec3d;
using Double4 = math::vec4d;

using Color4 = math::color4f;

using Int2 = math::vec2i;
using Int3 = math::vec3i;
using Int4 = math::vec4i;

using Mat3 = math::mat3f_c;
using Mat4 = math::mat4f_c;
using Trans4 = Mat4::right_transform;

using Microsoft::WRL::ComPtr;

class D3D12Exception : public std::runtime_error {
public:
	D3D12Exception(const std::string& what) : std::runtime_error(what.c_str()) {}
};

WZZ_D3D12_END
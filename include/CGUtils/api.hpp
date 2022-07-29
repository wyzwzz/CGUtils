#pragma once

#ifdef WZZ_ENABLE_OPENGL
#include "./api/opengl/attrib.hpp"
#include "./api/opengl/buffer.hpp"
#include "./api/opengl/framebuffer.hpp"
#include "./api/opengl/program.hpp"
#include "./api/opengl/renderbuffer.hpp"
#include "./api/opengl/sampler.hpp"
#include "./api/opengl/shader.hpp"
#include "./api/opengl/texture.hpp"
#include "./api/opengl/uniform.hpp"
#include "./api/opengl/vertex_array.hpp"
#include "./api/opengl/_window.hpp"
#include "./api/opengl/window.hpp"
#include "./api/opengl/demo.hpp"
#include "./api/opengl/camera.hpp"
#endif

#ifdef WZZ_ENABLE_VULKAN
#include "./api/vulkan/common.hpp"
#endif

//WZZ_ENABLE_D3D11

//WZZ_ENABLE_D3D12

#include "./api/imgui/imgui.h"
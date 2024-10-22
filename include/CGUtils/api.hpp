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

#ifdef WZZ_ENABLE_D3D11
#include "./api/d3d11/buffer.hpp"
#include "./api/d3d11/demo.hpp"
#include "./api/d3d11/device.hpp"
#include "./api/d3d11/device_context.hpp"
#include "./api/d3d11/input_layout.hpp"
#include "./api/d3d11/keyboard.hpp"
#include "./api/d3d11/mouse.hpp"
#include "./api/d3d11/pipeline_state.hpp"
#include "./api/d3d11/render_target.hpp"
#include "./api/d3d11/shader.hpp"
#include "./api/d3d11/texture2d.hpp"
#endif



#ifdef WZZ_ENABLE_D3D12


#endif

#include "./api/imgui/imgui.h"
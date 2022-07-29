#pragma once

#include <CGUtils/math.hpp>
#include <CGUtils/memory.hpp>
#include <CGUtils/misc.hpp>
#include <CGUtils/parallel.hpp>


#if defined(_WIN32) && !defined(VK_USE_PLATFORM_WIN32_KHR)
#define VK_USE_PLATFORM_WIN32_KHR
#endif


#include <volk.h>


#define WZZ_VK_BEGIN namespace wzz::vk{
#define WZZ_VK_END }

WZZ_VK_BEGIN
	using vec2 = math::vec2f;
	using vec3 = math::vec3f;
	using vec4 = math::vec4f;

	using vec2f = math::vec2f;
	using vec3f = math::vec3f;
	using vec4f = math::vec4f;

	using vec2i = math::vec2i;
	using vec3i = math::vec3i;
	using vec4i = math::vec4i;

	using vec2b = math::vec2b;
	using vec3b = math::vec3b;
	using vec4b = math::vec4b;

	using mat3 = math::mat3f_c;
	using mat4 = math::mat4f_c;

	enum QueueIndices
	{
		QUEUE_INDEX_GRAPHICS = 0,
		QUEUE_INDEX_COMPUTE,
		QUEUE_INDEX_TRANSFER,
		QUEUE_INDEX_VIDEO_DECODE,
		QUEUE_INDEX_COUNT
	};

WZZ_VK_END
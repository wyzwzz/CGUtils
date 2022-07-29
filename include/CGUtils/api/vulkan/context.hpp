#pragma once
#include "common.hpp"


WZZ_VK_BEGIN

struct queue_info_t
{
	queue_info_t();
	VkQueue queues[QUEUE_INDEX_COUNT] = {};
	uint32_t family_indices[QUEUE_INDEX_COUNT];
	uint32_t timestamp_valid_bits = 0;
};

class vk_context_t final: public misc::no_copy_t{
public:

private:
	VkDevice device = VK_NULL_HANDLE;
	VkInstance instance = VK_NULL_HANDLE;
	VkPhysicalDevice physical_device = VK_NULL_HANDLE;
	VolkDeviceTable device_table = {};

	VkPhysicalDeviceProperties gpu_props = {};
	VkPhysicalDeviceMemoryProperties mem_props = {};
	const VkApplicationInfo *user_application_info = nullptr;

	bool owned_instance = false;
	bool owned_device = false;


};


using vk_context_handle_t = misc::intrusive_ptr_t<vk_context_t>;

WZZ_VK_END
#pragma once
#include <cstdint>
namespace wzz::thread{

uint32_t get_current_thread_index();

void register_thread_index(uint32_t thread_index);

}
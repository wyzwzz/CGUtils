#pragma once

#include <string>
#include <string_view>

namespace wzz::stdstr{

inline bool ends_with(std::string_view str,std::string_view suffix) noexcept;

}

#include "impl/std_string.inl"
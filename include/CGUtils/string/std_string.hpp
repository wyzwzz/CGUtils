#pragma once

#include <string>
#include <string_view>
#include <sstream>
namespace wzz::stdstr{




inline bool ends_with(std::string_view str,std::string_view suffix) noexcept;

template <typename... Args>
inline std::string join(Args&&... args);

inline std::vector<std::string> split(const std::string& str,const char* delim,bool allow_empty = false);

inline std::string strip_whitespace(const std::string& str);

}

#include "impl/std_string.inl"
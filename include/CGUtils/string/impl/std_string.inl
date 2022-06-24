#pragma once
#include <cctype>
#include <iterator>
#include <sstream>

namespace wzz::stdstr{

inline bool ends_with(std::string_view str,std::string_view suffix) noexcept{
	if(str.size() < suffix.size()) return false;
	return str.substr(str.length() - suffix.length()) == suffix;
}


}
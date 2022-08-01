#pragma once
#include <cctype>
#include <iterator>
#include <sstream>

namespace wzz::stdstr{

namespace detail{

template <typename T>
void join_impl(std::ostringstream& stream,T&& t){
	stream << t;
}

template <typename T,typename... Args>
void join_impl(std::ostringstream& stream,T&& t, Args&&... args){
	stream << t;
	join_impl(stream,std::forward<Args>(args)...);
}

}

inline bool ends_with(std::string_view str,std::string_view suffix) noexcept{
	if(str.size() < suffix.size()) return false;
	return str.substr(str.length() - suffix.length()) == suffix;
}


template <typename... Args>
inline std::string join(Args&&... args){
	std::ostringstream stream;
	detail::join_impl(stream,args...);
	return stream.str();
}

inline std::vector<std::string> split(const std::string& str,const char* delim,bool allow_empty){
	if (str.empty())
		return {};
	std::vector<std::string> ret;

	size_t start_index = 0;
	size_t index = 0;
	while ((index = str.find_first_of(delim, start_index)) != std::string::npos)
	{
		if (allow_empty || index > start_index)
			ret.push_back(str.substr(start_index, index - start_index));
		start_index = index + 1;

		if (allow_empty && (index == str.size() - 1))
			ret.emplace_back();
	}

	if (start_index < str.size())
		ret.push_back(str.substr(start_index));
	return ret;
}

inline std::string strip_whitespace(const std::string& str){
	std::string ret;
	auto index = str.find_first_not_of(" \t");
	if (index == std::string::npos)
		return "";
	ret = str.substr(index, std::string::npos);
	index = ret.find_last_not_of(" \t");
	if (index != std::string::npos)
		return ret.substr(0, index + 1);
	else
		return ret;
}


}
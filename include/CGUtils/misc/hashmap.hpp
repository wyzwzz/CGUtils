#pragma once
#include <unordered_map>
#include "hash.hpp"

namespace wzz::misc{
struct unity_hasher_t{
	size_t operator()(size_t hash) const{
		return hash;
	}
};

template <typename T>
using hashmap_t = std::unordered_map<hash_t,T,unity_hasher_t>;

}
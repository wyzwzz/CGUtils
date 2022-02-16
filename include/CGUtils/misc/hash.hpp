#pragma once
#include <functional>
namespace wyz::misc{

namespace hash_impl{
    inline size_t hash_combine(size_t a, size_t b){
        if constexpr(sizeof(size_t) == 4)
            return a ^ (b + 0x9e3779b9 + (a << 6) + (a >> 2));
        else
            return a ^ (b + 0x9e3779b97f4a7c15 + (a << 6) + (a >> 2));
    }

    inline size_t hash(size_t h){
        return h;
    }

    template <typename T,typename... Others>
    size_t hash(size_t h, const T& t,const Others&... others){
        h = hash_combine(h, std::hash<T>()(t));
        return hash(h,others...);
    }
}

    template <typename T,typename... Others>
    size_t hash(const T& t, const Others&... others){
        return hash_impl::hash(std::hash<T>()(t),others...);
    }

}
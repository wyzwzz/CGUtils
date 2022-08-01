#pragma once
#include <functional>
#include <string>
namespace wzz::misc{

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

	using hash_t = size_t;

	class hasher_t{
	public:
		explicit hasher_t(hash_t h_)
		:h(h_)
		{

		}

		hasher_t() = default;

		template<typename T>
		void data(const T* data,size_t count){
			for(size_t i = 0; i < count; ++i)
				h = (h * 0x100000001b3ull) ^ data[i];
		}

		template <typename T>
		void hash(T value){
			h = ::wzz::misc::hash(value);
		}

		hash_t get() const{
			return h;
		}
		private:
		hash_t h = 0xcbf29ce484222325ull;
	};

}
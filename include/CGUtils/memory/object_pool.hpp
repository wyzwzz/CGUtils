#pragma once
#include "alloc.hpp"
#include <vector>
#include <memory>
#include <mutex>
namespace wzz::alloc{

template <typename T>
class object_pool_t{
public:
	template <typename... P>
	T* allocate(P&&... p){
		if(freed.empty()){
			uint32_t num_objects = 64u << mem.size();//increased alloc size
			T* ptr = static_cast<T*>(aligned_alloc(num_objects * sizeof(T),std::max(L1_CACHE_LINE_SIZE, alignof(T ))));

			if(!ptr){
				return nullptr;
			}

			for(auto i = 0u; i < num_objects; ++i)
				freed.emplace_back(ptr + i);

			mem.emplace_back(ptr);
		}
		auto ptr = freed.back();
		freed.pop_back();
		new(ptr) T(std::forward<P>(p)...);
		return ptr;
	}

	void free(T* ptr){
		//ptr should be valid that alloc in this, may add map to check
		ptr->~T();
		freed.push_back(ptr);
	}
	void clear(){
		freed.clear();
		mem.clear();
	}
	protected:
	std::vector<T*> freed;
	struct deleter_t{
		void operator()(T* ptr){
			//ptr->~T();//note user should free by hand
			aligned_free(ptr);
		}
	};
	//each element T* in mem is the first address of a T[N]
	std::vector<std::unique_ptr<T,deleter_t>> mem;
};

template <typename T>
class thread_safe_object_pool_t : private object_pool_t<T>{
public:
	template <typename... P>
	T* allocate(P&&... p){
		std::lock_guard<std::mutex> lk(mtx);
		return object_pool_t<T>::allocate(std::forward<P>(p)...);
	}
	void free(T* ptr){
		assert(ptr);
		ptr->~T();
		std::lock_guard<std::mutex> lk(mtx);
		this->freed.push_back(ptr);
	}
	void clear(){
		std::lock_guard<std::mutex> lk(mtx);
		object_pool_t<T>::clear();
	}
	private:
	std::mutex mtx;
};
}
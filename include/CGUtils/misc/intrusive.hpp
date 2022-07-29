#pragma once
#include "concept.hpp"

#include <cstddef>
#include <utility>
#include <memory>
#include <atomic>
#include <type_traits>

namespace wzz::misc{

class single_thread_counter{
public:
	void add_ref() noexcept{
		++count;
	}
	bool release() noexcept{
		return --count == 0;
	}
private:
	size_t count = 1;
};

class multi_thread_counter{
public:
	multi_thread_counter(){
		count.store(1,std::memory_order_relaxed);
	}

	void add_ref(){
		count.fetch_add(1,std::memory_order_relaxed);
	}

	bool release(){
		auto ret = count.fetch_sub(1,std::memory_order_acq_rel);
		return ret == 1;
	}
private:
	std::atomic_size_t count;
};

//-------------------------------
// intrusive_ptr_enabled_t<T> <-- T <-- derived U
// intrusive_ptr_t contains T*

template <typename T>
class intrusive_ptr_t;

template <typename T,typename Deleter = std::default_delete<T>,typename RefType = single_thread_counter>
class intrusive_ptr_enabled_t: no_copy_t
{
public:
	using instrusive_ptr_type_t = intrusive_ptr_t<T>;
	using enabled_base_t = T;
	using enabled_deleter_t = Deleter;
	using enabled_ref_t = RefType;

	void add_ref(){
		ref_count.add_ref();
	}

	void release_ref(){
		if(ref_count.release())
			Deleter()(static_cast<T*>(this));
	}

	intrusive_ptr_enabled_t() = default;

protected:
	instrusive_ptr_type_t ref_from_this();
private:
	enabled_ref_t ref_count;
};


template <typename T>
class intrusive_ptr_t{
public:
	template <typename U>
	friend class intrusive_ptr_t;

	intrusive_ptr_t() = default;



	using ref_base_t = intrusive_ptr_enabled_t<typename T::enabled_base_t,
											   typename T::enabled_deleter_t,
											   typename T::enabled_ref_t>;

	static_assert(std::is_base_of_v<ref_base_t,T>,"template class used intrusive_ptr_t should derived from intrusive_ptr_enabled_t");

	explicit intrusive_ptr_t(T* handle)
	:data(handle)
	{}

	T& operator*(){
		return *data;
	}

	T* operator->(){
		return data;
	}

	const T* operator->() const{
		return data;
	}

	explicit operator bool() const{
		return data != nullptr;
	}

	bool operator==(const intrusive_ptr_t& lhs) const{
		return data == lhs.data;
	}

	bool operator!=(const intrusive_ptr_t& lhs) const{
		return data != lhs.data;
	}

	T* get(){
		return data;
	}

	const T* get() const{
		return data;
	}

	void reset(){
		if(data){
			static_cast<ref_base_t*>(data)->release_ref();
		}
		data = nullptr;
	}

	template <typename U>
	intrusive_ptr_t<T>& operator=(const intrusive_ptr_t<U>& lhs){
		static_assert(std::is_base_of_v<T,U>, "cannot safe assign downcasted intrusive pointers");

		reset();

		data = static_cast<T*>(lhs.data);
		if(data)
			static_cast<ref_base_t*>(data)->add_ref();
		return *this;
	}

	intrusive_ptr_t& operator=(const intrusive_ptr_t& lhs){
		if(this != &lhs){
			reset();
			data = lhs.data;
			if(data)
				static_cast<ref_base_t*>(data)->add_ref();
		}
		return *this;
	}

	template <typename U>
	intrusive_ptr_t(const intrusive_ptr_t<U>& lhs){
		*this = lhs;
	}

	intrusive_ptr_t(const intrusive_ptr_t& lhs){
		*this = lhs;
	}

	~intrusive_ptr_t(){
		reset();
	}

	template <typename U>
	intrusive_ptr_t& operator=(intrusive_ptr_t<U>&& rhs) noexcept{
		static_assert(std::is_base_of_v<T,U>, "cannot safe assign downcasted intrusive pointers");
		reset();
		data = static_cast<T*>(rhs.data);
		rhs.data = nullptr;
		return *this;
	}

	intrusive_ptr_t& operator=(intrusive_ptr_t&& rhs) noexcept{
		if(this != &rhs){
			reset();
			data = rhs.data;
			rhs.data = nullptr;
		}
		return *this;
	}

	intrusive_ptr_t(intrusive_ptr_t&& rhs) noexcept{
		*this = std::move(rhs);
	}

	template <typename U>
	intrusive_ptr_t(intrusive_ptr_t<U>&& rhs) noexcept{
		*this = std::move(rhs);
	}

	T* release() &{
		T* ret = data;
		data = nullptr;
		return ret;
	}
	T* release() &&{
		T* ret = data;
		data = nullptr;
		return ret;
	}
	private:
	T* data = nullptr;

};

template <typename T, typename Deleter, typename RefType>
intrusive_ptr_t<T> intrusive_ptr_enabled_t<T, Deleter, RefType>::ref_from_this()
{
	add_ref();//ref count is not related with specific class T
	return intrusive_ptr_t<T>(static_cast<T*>(this));
}

template <typename Derived>
using devired_intrusive_ptr_t = intrusive_ptr_t<Derived>;

template <typename T,typename... P>
devired_intrusive_ptr_t<T> make_handle(P&&... p){
	return devired_intrusive_ptr_t<T>(new T(std::forward<P>(p)...));
}

template <typename Base,typename Derived,typename... P>
devired_intrusive_ptr_t<Base> make_derived_handle(P&&... p){
	return devired_intrusive_ptr_t<Base>(new Derived(std::forward<P>(p)...));
}

template <typename T>
using thread_safe_intrusive_ptr_enabled = intrusive_ptr_enabled_t<T,std::default_delete<T>,multi_thread_counter>;

}
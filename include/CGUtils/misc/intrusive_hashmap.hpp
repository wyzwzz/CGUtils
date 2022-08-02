#pragma once

#include "hash.hpp"
#include "intrusive_list.hpp"
#include <CGUtils/memory/object_pool.hpp>
#include <CGUtils/parallel/read_write_lock.hpp>
#include <cassert>
#include <vector>

namespace wzz::misc{

template <typename T>
class intrusive_hashmap_enabled_t:public intrusive_list_enabled_t<T>{
public:
	intrusive_hashmap_enabled_t() = default;
	intrusive_hashmap_enabled_t(hash_t hash)
	:intrusive_hashmap_key(hash)
	{}

	void set_hash(hash_t hash){
		intrusive_hashmap_key = hash;
	}
	hash_t get_hash() const{
		return intrusive_hashmap_key;
	}
private:
	hash_t intrusive_hashmap_key = 0;
};

template <typename T>
struct intrusive_pod_wrapper_t:public intrusive_hashmap_enabled_t<intrusive_pod_wrapper_t<T>>{
	template <typename U>
	explicit intrusive_pod_wrapper_t(U&& value_)
	  : value(std::forward<U>(value_))
	{
	}

	intrusive_pod_wrapper_t() = default;

	T& get()
	{
		return value;
	}

	const T& get() const
	{
		return value;
	}

	T value = {};
};

// This HashMap is non-owning. It just arranges a list of pointers.
// It's kind of special purpose container used by the Vulkan backend.
// Dealing with memory ownership is done through composition by a different class.
// T must inherit from IntrusiveHashMapEnabled<T>.
// Each instance of T can only be part of one hashmap.
template <typename T>
class intrusive_hashmap_holder_t{
public:
	static_assert(std::is_base_of_v<intrusive_list_enabled_t<T>,T>,"T must derived from intrusive_list_enabled_t<T>");
	enum { InitialSize = 16, InitialLoadCount = 3 };

	T* find(hash_t hash) const noexcept{
		if(values.empty())
			return nullptr;
		hash_t hash_mask = values.size() - 1;
		//values.size() must be power of 2
		assert((hash_mask & (hash_mask + 1)) == 0);
		auto masked = hash & hash_mask;
		for(uint32_t i = 0; i < load_count; ++i){
			if(values[masked] && get_hash(values[masked]) == hash)
				return values[masked];
			masked = (masked + 1) & hash_mask;
		}
		return nullptr;
	}

	template <typename P>
	bool find_and_consume_pod(hash_t hash, P &p) const
	{
		//T should be an instance of template class intrusive_pod_wrapper_t
		T *t = find(hash);
		if (t)
		{
			p = t->get();
			return true;
		}
		else
			return false;
	}

	//if value already exists, it will replace the old and return value
	//if not, insert and return nullptr
	T* insert_replace(T* value){
		if(values.empty())
			grow();
		auto hash = get_hash(value);
		hash_t hash_mask = values.size() - 1;
		//values.size() must be power of 2
		assert((hash_mask & (hash_mask + 1)) == 0);
		auto masked = hash & hash_mask;
		for(uint32_t i = 0; i < load_count; ++i){
			if(values[masked] && get_hash(values[masked]) == hash){
				std::swap(values[masked],value);
				list.erase(value);
				list.insert_front(values[masked]);
				return value;
			}
			else if(!values[masked]){
				values[masked] = value;
				list.insert_front(value);
				return nullptr;
			}
			masked = (masked + 1) & hash_mask;
		}

		grow();
		return insert_replace(value);
	}


	//if value already exists, insert will not happen, value will change to the one already in hashmap,
	//and return the original value, these two must be of same hash key but may not the same value
	//if value not exists, insert will success and return nullptr
	T* insert_yield(T* &value){
		if(values.empty())
			grow();
		auto hash = get_hash(value);
		hash_t hash_mask = values.size() - 1;
		//values.size() must be power of 2
		assert((hash_mask & (hash_mask + 1)) == 0);
		auto masked = hash & hash_mask;
		for(uint32_t i = 0; i < load_count; ++i){
			if(values[masked] && get_hash(values[masked]) == hash){
				auto ret = value;
				value = values[masked];
				return ret;
			}
			else if(!values[masked]){
				values[masked] = value;
				list.insert_front(value);
				return nullptr;
			}
			masked = (masked + 1) & hash_mask;
		}
		grow();
		return insert_yield(value);
	}

	T* erase(hash_t hash){
		hash_t hash_mask = values.size() - 1;
		auto masked = hash & hash_mask;
		for(uint32_t i = 0; i < load_count; ++i){
			if(values[masked] && get_hash(values[masked]) == hash){
				auto value = values[masked];
				list.erase(value);
				values[masked] = nullptr;
				return value;
			}
			masked = (masked + 1) & hash_mask;
		}
		return nullptr;
	}

	void erase(T* value){
		erase(get_hash(value));
	}

	//remove all elements and leaving the container with a size of 0
	void clear(){
		list.clear();
		values.clear();
		load_count = 0;
	}

	typename intrusive_list_t<T>::iterator_t begin() const{
		return list.begin();
	}
	typename intrusive_list_t<T>::iterator_t end() const{
		return list.end();
	}
	intrusive_list_t<T>& inner_list(){
		return list;
	}
	const intrusive_list_t<T>& inner_list() const{
		return list;
	}
private:
	inline hash_t get_hash(const T* value) const noexcept{
		return static_cast<const intrusive_hashmap_enabled_t<T>*>(value)->get_hash();
	}
	inline hash_t get_hash_for_index(hash_t masked) const{
		return get_hash(values[masked]);
	}
	inline bool compare_key(hash_t masked,hash_t hash) const{
		return get_hash_for_index(masked) == hash;
	}

	bool insert_inner(T* value){
		hash_t hash_mask = values.size() - 1;
		assert((hash_mask & (hash_mask + 1)) == 0);
		auto hash = get_hash(value);
		auto masked = hash & hash_mask;
		for(uint32_t i = 0; i < load_count; ++i){
			if(!values[masked]){
				values[masked] = value;
				return true;
			}
			masked = (masked + 1) & hash_mask;
		}
		return false;
	}

	void grow(){
		bool ok = false;
		do{
			std::fill(values.begin(),values.end(),nullptr);
			if(values.empty()){
				values.resize(InitialSize);
				load_count = InitialLoadCount;
			}
			else{
				values.resize(values.size() * 2);
				load_count++;//just increase one is enough in most case
			}
			// re-insert
			ok = true;
			for(auto& t:list){
				if(!insert_inner(&t)){
					ok = false;
					break;
				}
			}
		}while(!ok);
	}

private:
	std::vector<T*> values;
	intrusive_list_t<T> list;
	uint32_t load_count = 0;//max load count for same hash key but different value
};

template <typename T>
class intrusive_hashmap_t: no_copy_t{
public:
	static_assert(std::is_base_of_v<intrusive_list_enabled_t<T>,T>,"T must derived from intrusive_list_enabled_t<T>");

	intrusive_hashmap_t() = default;
	~intrusive_hashmap_t(){
		clear();
	}

	void clear(){
		auto& list = hashmap.inner_list();
		for(auto it = list.begin();it != list.end();){
			auto to_free = it.get();
			it = list.erase(it);
			pool.free(to_free);
		}
		hashmap.clear();
	}

	T* find(hash_t hash)  const{
		return hashmap.find(hash);
	}

	T& operator[](hash_t hash){
		auto ret = find(hash);
		if(!ret)
			ret = emplace_yield(hash);
		return *ret;
	}

	template <typename P>
	bool find_and_consume_pod(hash_t hash, P &p) const
	{
		return hashmap.find_and_consume_pod(hash, p);
	}

	void erase(T* value){
		hashmap.erase(value);
		pool.free(value);
	}

	void erase(hash_t hash){
		auto value = hashmap.erase(hash);
		if(value)
			pool.free(value);
	}

	template <typename... Args>
	T* emplace_replace(hash_t hash,Args&&... args){
		auto ret = allocate(std::forward<Args>(args)...);
		return insert_replace(hash,ret);
	}
	template <typename... Args>
	T* emplace_yield(hash_t hash,Args&&... args){
		auto ret = allocate(std::forward<Args>(args)...);
		return insert_yield(hash,ret);
	}
	template <typename... Args>
	T* allocate(Args&&... args){
		return pool.allocate(std::forward<Args>(args)...);
	}
	void free(T* value){
		pool.free(value);
	}
	T* insert_replace(hash_t hash,T* value){
		static_cast<intrusive_hashmap_enabled_t<T>*>(value)->set_hash(hash);
		auto to_delete = hashmap.insert_replace(value);
		if(to_delete)
			pool.free(to_delete);
		return value;
	}
	T* insert_yield(hash_t hash,T* value){
		static_cast<intrusive_hashmap_enabled_t<T>*>(value)->set_hash(hash);
		auto to_delete = hashmap.insert_yield(value);
		if(to_delete)
			pool.free(to_delete);
		return value;
	}

	typename intrusive_list_t<T>::iterator_t begin() const{
		return hashmap.begin();
	}
	typename intrusive_list_t<T>::iterator_t end() const{
		return hashmap.end();
	}
	intrusive_hashmap_t& get_thread_unsafe(){
		return *this;
	}
	const intrusive_hashmap_t& get_thread_unsafe() const {
		return *this;
	}
private:
	intrusive_hashmap_holder_t<T> hashmap;
	alloc::object_pool_t<T> pool;
};

template <typename T>
using intrusive_hashmap_wrapper_t = intrusive_hashmap_t<intrusive_pod_wrapper_t<T>>;

template <typename T>
class thread_safe_intrusive_hashmap_t{
public:
	T* find(hash_t hash) const{
		lock.lock_read();
		auto ret = hashmap.find(hash);
		lock.unlock_read();
		return ret;
	}

	template <typename P>
	bool find_and_consume_pod(hash_t hash,P& p) const{
		lock.lock_read();
		auto ret = hashmap.find_and_consume_pod(hash,p);
		lock.unlock_read();
		return ret;
	}

	void clear(){
		lock.lock_write();
		hashmap.clear();
		lock.unlock_write();
	}

	//T* return by find should not be used by any other when call this method
	void erase(T* value){
		lock.lock_write();
		hashmap.erase(value);
		lock.unlock_write();
	}
	void erase(hash_t hash)
	{
		lock.lock_write();
		hashmap.erase(hash);
		lock.unlock_write();
	}
	template <typename... P>
	T *allocate(P&&... p)
	{
		lock.lock_write();
		T *t = hashmap.allocate(std::forward<P>(p)...);
		lock.unlock_write();
		return t;
	}
	void free(T *value)
	{
		lock.lock_write();
		hashmap.free(value);
		lock.unlock_write();
	}

	T *insert_replace(hash_t hash, T *value)
	{
		lock.lock_write();
		value = hashmap.insert_replace(hash, value);
		lock.unlock_write();
		return value;
	}

	T *insert_yield(hash_t hash, T *value)
	{
		lock.lock_write();
		value = hashmap.insert_yield(hash, value);
		lock.unlock_write();
		return value;
	}
	//be care of using, it may free pointer get by reader
	template <typename... P>
	T *emplace_replace(hash_t hash, P&&... p)
	{
		lock.lock_write();
		T *t = hashmap.emplace_replace(hash, std::forward<P>(p)...);
		lock.unlock_write();
		return t;
	}

	template <typename... P>
	T *emplace_yield(hash_t hash, P&&... p)
	{
		lock.lock_write();
		T *t = hashmap.emplace_yield(hash, std::forward<P>(p)...);
		lock.unlock_write();
		return t;
	}

	typename intrusive_list_t<T>::iterator_t begin() const{
		return hashmap.begin();
	}
	typename intrusive_list_t<T>::iterator_t end() const{
		return hashmap.end();
	}
	intrusive_hashmap_t<T>& get_thread_unsafe(){
		return hashmap;
	}
	const intrusive_hashmap_t<T>& get_thread_unsafe() const {
		return hashmap;
	}
private:
	intrusive_hashmap_t<T> hashmap;
	mutable thread::rw_spinlock_t lock;
};

// A special purpose hashmap which is split into a read-only, immutable portion and a plain thread-safe one.
// User can move read-write thread-safe portion to read-only portion when user knows it's safe to do so.
template <typename T>
class thread_safe_intrusive_hashmap_read_cached_t{
public:
	~thread_safe_intrusive_hashmap_read_cached_t(){
		clear();
	}

	T* find(hash_t hash) const{
		auto ret = read_only.find(hash);
		if(ret)
			return ret;
		lock.lock_read();
		ret = read_write.find(hash);
		lock.unlock_read();
		return ret;
	}

	template <typename P>
	bool find_and_consume_pod(hash_t hash, P &p) const
	{
		if (read_only.find_and_consume_pod(hash, p))
			return true;

		lock.lock_read();
		bool ret = read_write.find_and_consume_pod(hash, p);
		lock.unlock_read();
		return ret;
	}

	template <typename... P>
	T *allocate(P&&... p)
	{
		lock.lock_write();
		T *t = object_pool.allocate(std::forward<P>(p)...);
		lock.unlock_write();
		return t;
	}

	void free(T *ptr)
	{
		lock.lock_write();
		object_pool.free(ptr);
		lock.unlock_write();
	}
	T *insert_yield(hash_t hash, T *value)
	{
		static_cast<intrusive_hashmap_enabled_t<T> *>(value)->set_hash(hash);//value->set_hash(hash) is also ok
		lock.lock_write();
		T *to_delete = read_write.insert_yield(value);
		if (to_delete)
			object_pool.free(to_delete);
		lock.unlock_write();
		return value;
	}

	template <typename... P>
	T *emplace_yield(hash_t hash, P&&... p)
	{
		T *t = allocate(std::forward<P>(p)...);
		return insert_yield(hash, t);
	}

	intrusive_hashmap_holder_t<T>& get_read_only(){
		return read_only;
	}

	intrusive_hashmap_holder_t<T>& get_read_write(){
		return read_write;
	}
	//user should know it's safa to call this method
	void move_to_read_only(){
		auto& list = read_write.inner_list();
		for(auto it = list.begin(); it != list.end();){
			auto to_move = it.get();
			read_write.erase(it);
			auto to_delete = read_only.insert_yield(to_move);
			if(to_delete)
				object_pool.free(to_delete);
			//read_write.erase(it) will also erase it in list
			it = list.begin();
		}
	}

	void clear(){
		lock.lock_write();
		clear_list(read_only.inner_list());
		clear_list(read_write.inner_list());
		read_only.clear();
		read_write.clear();
		lock.unlock_write();
	}
private:
	void clear_list(intrusive_list_t<T>& list){
		for(auto it = list.begin();it != list.end();){
			auto to_free = it.get();
			object_pool.free(to_free);
			it = list.erase(it);
		}
	}
private:
	intrusive_hashmap_holder_t<T> read_only;
	intrusive_hashmap_holder_t<T> read_write;
	alloc::object_pool_t<T> object_pool;
	mutable thread::rw_spinlock_t lock;
};

template <typename T>
class temporary_hashmap_enabled_t{
public:
	void set_hash(hash_t hash_){
		hash = hash_;
	}
	void set_index(uint32_t index_){
		index = index_;
	}
	hash_t get_hash() const {
		return hash;
	}
	uint32_t get_index() const {
		return index;
	}
private:
	hash_t hash = 0;
	uint32_t index = 0;
};

// special hashmap will only store key-value data in last RingSize frames
template <typename T,unsigned int RingSize = 4, bool ReuseObjects = false>
class temporary_hashmap_t{
public:
	static_assert(std::is_base_of_v<temporary_hashmap_enabled_t<T>,T>,"");
	static_assert((RingSize & (RingSize - 1)) == 0,"RingSize must be power of two");
	~temporary_hashmap_t(){
		clear();
	}
	void clear(){
		for(auto& ring:rings){
			for(auto& node:ring)
				object_pool.free(static_cast<T*>(node));
			ring.clear();
		}
		hashmap.clear();
		for(auto& vacant:vacants)
			object_pool.free(static_cast<T*>(&*vacant));
		vacants.clear();
		object_pool.clear();
	}
	void begin_frame(){
		index = (index + 1) & (RingSize - 1);
		for(auto& node:rings[index]){
			hashmap.erase(static_cast<temporary_hashmap_enabled_t<T>*>(&node)->get_hash());
			free_object(&node);
		}
		rings[index].clear();
	}
	T* request(hash_t hash){
		auto value = hashmap.find(hash);
		if(value){
			auto node = value->get();
			auto node_idx = static_cast<temporary_hashmap_enabled_t<T>*>(node)->get_index();
			if(node_idx != index){
				rings[index].move_to_front(rings[node_idx], node);
				static_cast<temporary_hashmap_enabled_t<T>*>(node)->set_index(index);
			}
			return &*node;
		}
		else
			return nullptr;
	}

	template <typename... Args>
	void make_vacant(Args&&... args){
		vacants.push_back(object_pool.allocate(std::forward<Args>(args)...));
	}

	T* request_vacant(hash_t hash){
		if(vacants.empty())
			return nullptr;

		auto top = vacants.back();
		vacants.pop_back();
		static_cast<temporary_hashmap_enabled_t<T>*>(top)->set_index(index);
		static_cast<temporary_hashmap_enabled_t<T>*>(top)->set_index(hash);
		hashmap.emplace_replace(hash,top);
		rings[index].insert_front(top);
		return &*top;
	}

	template<typename... Args>
	T* emplace(hash_t hash,Args&&... args){
		auto node = object_pool.allocate(std::forward<Args>(args)...);
		static_cast<temporary_hashmap_enabled_t<T>*>(node)->set_index(index);
		static_cast<temporary_hashmap_enabled_t<T>*>(node)->set_hash(hash);
		hashmap.emplace_replace(hash,node);
		rings[index].insert_front(node);
		return node;
	}
private:
	void free_object(T* object){
		if constexpr(ReuseObjects){
			vacants.push_back(object);
		}
		else{
			object_pool.free(object);
		}
	}
private:
	intrusive_list_t<T> rings[RingSize];
	alloc::object_pool_t<T> object_pool;
	uint32_t index = 0;
	using iterator_t = typename intrusive_list_t<T>::iterator_t;
	intrusive_hashmap_t<intrusive_pod_wrapper_t<iterator_t>> hashmap;
	std::vector<iterator_t> vacants;

};

}
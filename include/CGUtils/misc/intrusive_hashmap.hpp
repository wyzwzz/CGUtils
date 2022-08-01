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

private:

};

template <typename T>
struct intrusive_pod_wrapper:public intrusive_hashmap_enabled_t<intrusive_pod_wrapper<T>>{

};

template <typename T>
class intrusive_hashmap_holder_t{
public:

private:

};

template <typename T>
class intrusive_hashmap_t{
public:

private:

};

template <typename T>
using intrusive_hashmap_wrapper_t = intrusive_hashmap_t<intrusive_pod_wrapper<T>>;

template <typename T>
class thread_safe_intrusive_hashmap_t{
public:

private:
};

template <typename T>
class thread_safe_intrusive_hashmap_read_cached_t{
public:

private:

};

template <typename T>
class temporary_hashmap_enabled_t{
public:

private:

};

template <typename T,unsigned int RingSize = 4, bool ReuseObjects = false>
class temporary_hashmap_t{
public:

private:


};

}
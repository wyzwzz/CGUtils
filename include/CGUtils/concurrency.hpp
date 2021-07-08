//
// Created by wyz on 2021/7/5.
//

#ifndef CGUTILS_CONCURRENCY_HPP
#define CGUTILS_CONCURRENCY_HPP
#include <list>
#include <condition_variable>
template<typename T>
class ConcurrentQueue{
public:

private:
    std::list<T> m_queue;
    std::mutex m_mtx;
    size_t m_queue_size;
};

#endif //CGUTILS_CONCURRENCY_HPP

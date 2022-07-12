#pragma once
#include <list>
#include "alloc.hpp"
namespace wzz::alloc{
    class alignas(64) memory_arena_t{
    public:
        memory_arena_t(size_t max_block_size = 262144):max_block_size(max_block_size){}
        memory_arena_t(const memory_arena_t &) = delete;
        memory_arena_t &operator=(const memory_arena_t &) = delete;
        ~memory_arena_t() {
            aligned_free(current_block);
            for (auto &block : used_blocks) aligned_free(block.second);
            for (auto &block : available_blocks) aligned_free(block.second);
        }
        void* alloc(size_t nBytes){
            return _alloc(nBytes);
        }
        void* _alloc(size_t nBytes){
            const int align = alignof(std::max_align_t);
            static_assert( align > 0 && !(align & (align - 1)), "Minimum alignment not a power of two");
            nBytes = (nBytes + align - 1) & ~(align - 1);
            if(current_block_pos + nBytes > current_alloc_size){
                //alloc new block
                if(current_block){
                    used_blocks.emplace_back(current_alloc_size,current_block);
                    current_block = nullptr;
                    current_alloc_size = 0;
                }
                for(auto it = available_blocks.begin(); it != available_blocks.end();it++){
                    if(it->first >= nBytes){
                        current_alloc_size = it->first;
                        current_block = it->second;
                        available_blocks.erase(it);
                        break;
                    }
                }
                if(!current_block){
                    current_alloc_size = std::max(nBytes,max_block_size);
                    current_block = aligned_alloc<uint8_t>(current_alloc_size);
                }
                current_block_pos = 0;
            }
            void* ret = current_block + current_block_pos;
            current_block_pos += nBytes;
            return ret;
        }
        template <typename T>
        T *alloc(size_t n = 1, bool run_constructor = true) {
            T *ret = (T *)_alloc(n * sizeof(T));
            if (run_constructor)
                for (size_t i = 0; i < n; ++i) new (&ret[i]) T();
            return ret;
        }

        template<typename T,typename... Args>
        T* alloc_object(Args&&... args){
            T *ret = (T*)_alloc(sizeof(T));
            new (ret) T(std::forward<Args>(args)...);
            return ret;
        }

        void reset() {
            current_block_pos = 0;
            available_blocks.splice(available_blocks.begin(), used_blocks);
        }
        size_t total_allocated() const {
            size_t total = current_alloc_size;
            for (const auto &alloc : used_blocks) total += alloc.first;
            for (const auto &alloc : available_blocks) total += alloc.first;
            return total;
        }
        size_t used_bytes() const{
            size_t used = 0;
            for(const auto& alloc:used_blocks) used += alloc.first;
            return used;
        }

    private:
        const size_t max_block_size;
        size_t current_block_pos = 0, current_alloc_size = 0;
        uint8_t *current_block = nullptr;
        std::list<std::pair<size_t, uint8_t *>> used_blocks, available_blocks;
    };
}
//
// Created by wyz on 2022/6/10.
//
#pragma once

namespace wzz::misc{

    class no_copy_t{
      public:
        no_copy_t() = default;
        no_copy_t(const no_copy_t&) = delete;
        no_copy_t& operator=(const no_copy_t&) = delete;
        no_copy_t(no_copy_t&&) noexcept = default;
        no_copy_t& operator=(no_copy_t&&) noexcept = default;
    };

    class no_move_t{
      public:
        no_move_t() = default;
        no_move_t(const no_move_t&) = default;
        no_move_t& operator=(const no_move_t&) = default;
        no_move_t(no_move_t&&) noexcept = delete;
        no_move_t& operator=(no_move_t&&) noexcept = delete;
    };

    class no_heap_t{
      public:
        void* operator new(size_t) = delete;
        void* operator new[](size_t) = delete;
    };

	template <typename T>
	struct no_init_pod_t{
		T value;
		no_init_pod_t(){
			static_assert(sizeof(no_init_pod_t<T>) == sizeof(T), "Sizes do not match.");
			static_assert(alignof(no_init_pod_t<T>) == alignof(T), "Alignments do not match.");
		}
		static_assert(std::is_pod<T>::value, "Type is not POD.");
	};
	}
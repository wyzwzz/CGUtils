#pragma once

#if defined(_WIN32)
#define WZZ_OS_WIN32
#elif defined(__linux)
#define WZZ_OS_LINUX
#endif

// CXX

#if defined(_MSC_VER)
#define WZZ_CXX_MSVC
#elif defined(__clang__)
#define WZZ_CXX_CLANG
#define WZZ_CXX_IS_GNU
#elif defined(__GNUC__)
#define WZZ_CXX_GCC
#define WZZ_CXX_IS_GNU
#endif

// DEBUG

#ifndef NDEBUG
#define WZZ_DEBUG
#define WZZ_WHEN_DEBUG(op) do { op; } while(false)
#else
#define WZZ_WHEN_DEBUG(op) do { } while(false)
#endif
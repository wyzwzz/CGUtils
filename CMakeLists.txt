cmake_minimum_required(VERSION 3.16)
project(CGUtils LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

option(BUILD_TEST "" OFF)

file(
        GLOB_RECURSE CGUTILS_SRCS
        "${PROJECT_SOURCE_DIR}/include/*.hpp"
        "${PROJECT_SOURCE_DIR}/include/*.inl"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_library(CGUtils STATIC ${CGUTILS_SRCS})

target_include_directories(
        CGUtils
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(
        CGUtils
        PRIVATE
        cxx_std_17
)

if(BUILD_TEST)
    add_subdirectory(test)
endif()
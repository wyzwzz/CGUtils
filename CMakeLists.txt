cmake_minimum_required(VERSION 3.16)
project(CGUtils LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)

option(BUILD_TEST "" OFF)

option(BUILD_ENABLE_OPENGL "" ON)

include(${PROJECT_SOURCE_DIR}/third_party/spdlog.cmake)

file(
        GLOB_RECURSE CGUTILS_SRCS
        "${PROJECT_SOURCE_DIR}/include/*.hpp"
        "${PROJECT_SOURCE_DIR}/include/*.inl"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        ${PROJECT_SOURCE_DIR}/ext/glad/src/glad.c
)

add_library(CGUtils STATIC ${CGUTILS_SRCS})

if(BUILD_ENABLE_OPENGL)
add_compile_definitions(
        WZZ_ENABLE_OPENGL
)
endif()

target_link_libraries(
        CGUtils
        PUBLIC
        spdlog::spdlog
)

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

target_include_directories(CGUtils PUBLIC ${PROJECT_SOURCE_DIR}/ext/glad/include)

add_subdirectory(ext/glfw)

target_link_libraries(
        CGUtils
        PUBLIC
        glfw
)

if(BUILD_TEST)
    add_subdirectory(test)

    add_subdirectory(tool_test)

    add_subdirectory(graphics_api_test)

endif()
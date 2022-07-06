
add_executable(opengl_test main.cpp)

target_link_libraries(opengl_test PRIVATE
        CGUtils)

target_compile_features(
        opengl_test
        PRIVATE
        cxx_std_20
)
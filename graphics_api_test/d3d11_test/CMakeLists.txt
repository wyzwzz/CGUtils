
set(BUILD_ENABLE_D3D11 ON)


add_executable(d3d11_test main.cpp)

target_link_libraries(d3d11_test
    PRIVATE CGUtils
)


target_compile_features(
        d3d11_test
        PRIVATE
        cxx_std_20
)

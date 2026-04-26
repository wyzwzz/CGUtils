
set(BUILD_ENABLE_D3D12 ON)


add_executable(d3d12_test main.cpp)

target_link_libraries(d3d12_test
    PRIVATE CGUtils
)


target_compile_features(
        d3d12_test
        PRIVATE
        cxx_std_20
)

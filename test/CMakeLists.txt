include_directories(${PROJECT_SOURCE_DIR}/include)
add_executable(test_camera test_camera.cpp)
add_executable(test_lru test_lru.cpp)
add_executable(test_ref test_ref.cpp)
add_executable(test_singleton test_singleton.cpp)
add_executable(test_timer test_timer.cpp)
add_executable(test_progressbar test_progressbar.cpp)

add_library(liba a.cpp b.cpp)
add_executable(test_def main.cpp)
target_link_libraries(test_def PRIVATE liba)

add_executable(test_math test_math.cpp)
target_link_libraries(test_math PRIVATE CGUtils)
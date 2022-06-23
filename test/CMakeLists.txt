include(${PROJECT_SOURCE_DIR}/third_party/googletest.cmake)
include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE CGUTILS_TEST_SOURCES
        *.cpp
        )

add_executable(CGUtils_test_all ${CGUTILS_TEST_SOURCES})

include(GoogleTest)
gtest_add_tests(CGUtils_test_all "" AUTO)

target_link_libraries(
        CGUtils_test_all
        PRIVATE
        CGUtils
        GTest::gtest_main GTest::gtest GTest::gmock GTest::gmock_main
)





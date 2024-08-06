cmake_minimum_required(VERSION 3.20)
include(FetchContent)

FetchContent_Declare(
    gooletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG ff233bdd4cac0a0bf6e5cd45bda3406814cb2796
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)


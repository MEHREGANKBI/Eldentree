cmake_minimum_required(VERSION 3.20)
include(FetchContent)

FetchContent_Declare(googletest
GIT_REPOSITORY https://github.com/google/googletest.git
GIT_TAG ff233bdd4cac0a0bf6e5cd45bda3406814cb2796)

FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(test_process_eqp test_process_event_queue_pairs.cpp)

target_include_directories(test_process_eqp PRIVATE ./../../libqeh/include/)

target_link_libraries(
    test_process_eqp
    PRIVATE
    GTest::GTest
    qeh)


add_test(libqeh_test_process_eqp test_process_eqp)
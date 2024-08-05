cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(qeh VERSION 1.0.0)

add_library(${PROJECT_NAME} STATIC src/queued_event_handler.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include/)
cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
project(Eldentree VERSION 1.0.0)

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)
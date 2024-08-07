cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(qeh VERSION 1.0.0)

# Compiler requirements.
if (NOT(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang"))
    message(FATAL_ERROR "Your compiler is not supported. Switch to GNU or Clang.")
endif()

execute_process(COMMAND "uname" "-o" OUTPUT_VARIABLE OPERATING_SYSTEM)
execute_process(COMMAND "arch" OUTPUT_VARIABLE CPU_ARCH)
# check OS and hardware architecure requirements.
if ((NOT(OPERATING_SYSTEM MATCHES "GNU/Linux")) OR (NOT(CPU_ARCH MATCHES "x86_64")))
    message(FATAL_ERROR "Currently, only the amd64 version of GNU/Linux is supported.")
endif()

add_library(${PROJECT_NAME} STATIC src/queued_event_handler.cpp src/supplementary_assets.cpp include/queued_event_handler.hpp)
target_include_directories(${PROJECT_NAME} PRIVATE include/)
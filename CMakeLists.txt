cmake_minimum_required(VERSION 4.0)
project(Memory_manager)

set(CMAKE_CXX_STANDARD 11)

add_executable(Memory_manager 44_02_08main.cpp
        os_mem.h
        44_02_08task.cpp
        44_02_08task.cpp)
target_compile_options(Memory_manager PRIVATE -fsanitize=address)
target_link_options(Memory_manager PRIVATE -fsanitize=address)
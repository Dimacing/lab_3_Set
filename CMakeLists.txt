cmake_minimum_required(VERSION 3.19)
project(mephi_lab3)

set(CMAKE_CXX_STANDARD 20)

add_executable(mephi_lab3 main.cpp tests.hpp functions.hpp Heap_Array.hpp Heap_Node.hpp Set_Array.hpp Set_Node.hpp)
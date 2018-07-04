cmake_minimum_required(VERSION 3.10)
project(Fork__Exec C)

set(CMAKE_C_STANDARD 99)

add_executable(Counter counter.c)
add_executable(Fork__Exec main.c)
cmake_minimum_required(VERSION 3.10)
project(inotify_for_mv C)

set(CMAKE_C_STANDARD 99)

add_executable(inotify_for_mv utils.c utils.h event_queue.h event_queue.c main.c)
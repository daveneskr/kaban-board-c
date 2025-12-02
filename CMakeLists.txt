cmake_minimum_required(VERSION 3.30)
project(SE_Assignment_3 C)

set(CMAKE_C_STANDARD 11)

add_executable(SE_Assignment_3 main.c
        useful.c
        useful.h
        kanban.c
        kanban.h
        user_input.c
        user_input.h
        ascii_art.c
        ascii_art.h)

cmake_minimum_required(VERSION 3.16.3)
project(MiniLaska C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic")
add_executable(MiniLaska main.c minilaska.c minimax.c)
cmake_minimum_required(VERSION 3.21)
project(clox C)

set(CMAKE_C_STANDARD 99)

add_executable(clox main.c common.h chunk.h chunk.c memory.h memory.c debug.c debug.h value.h value.c vm.c vm.h compiler.h compiler.c scanner.c scanner.h object.h object.c table.h table.c utest.h tests.c)

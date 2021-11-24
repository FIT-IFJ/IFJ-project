cmake_minimum_required(VERSION 3.20)
project(untitled C)

set(CMAKE_C_STANDARD 99)

add_executable(untitled parser.c)
add_library(mypars parser.c parser.h)
add_library(myscann scanner.c scanner.h)
add_library(mydyn dynamic_string.c dynamic_string.h)
target_link_libraries(untitled PRIVATE mypars myscann mydyn)


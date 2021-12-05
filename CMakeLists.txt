cmake_minimum_required(VERSION 3.20)
project(IFJ_project_main C)

set(CMAKE_C_STANDARD 11)

add_executable(IFJ_project_main parser.c)
add_library(mypars parser.c parser.h)
add_library(myprece precedence_parser.c precedence_parser.h)
add_library(myscann scanner.c scanner.h)
add_library(mydyn dynamic_string.c dynamic_string.h)
add_library(mystruct structures.c structures.h)
add_library(mysymt symtab.c symtab.h)
add_library(myast ast.c ast.h)

target_link_libraries(IFJ_project_main PRIVATE mypars myprece myscann mydyn mystruct mysymt myast)
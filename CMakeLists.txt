cmake_minimum_required(VERSION 3.30)
project(untitled C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ferror-limit=999")



add_library(untitled STATIC
        Source_Files/assembler.c
        Header_Files/utils.h
        Source_Files/utils.c
        Header_Files/first_pass.h
        Source_Files/first_pass.c
        Source_Files/symbol_table.c
        Header_Files/symbol_table.h
        Header_Files/second_pass.h
        Source_Files/second_pass.c
        Header_Files/codegen.h
        Source_Files/codegen.c
)

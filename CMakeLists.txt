cmake_minimum_required(VERSION 3.30)
project(untitled C)

set(CMAKE_C_STANDARD 90)

add_library(untitled STATIC Source_Files/library.c
        Source_Files/assembler.c
        Source_Files/macro_proc.c
        Header_Files/utils.h
        Source_Files/utils.c
        Header_Files/errors.h
        Source_Files/errors.c
        Header_Files/first_pass.h
        Source_Files/first_pass.c
        Source_Files/symbol_table.c
        Header_Files/symbol_table.h
        Source_Files/parser.c
        Header_Files/parser.h
        Header_Files/second_pass.h
        Source_Files/second_pass.c
        Header_Files/output.h
        Source_Files/output.c
        Header_Files/globals.h
        Header_Files/codegen.h
        Source_Files/codegen.c)

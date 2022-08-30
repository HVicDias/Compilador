cmake_minimum_required(VERSION 3.23)
project(Compiler)

set(CMAKE_CXX_STANDARD 23)

add_executable(Compiler source/main.cpp libs/lexical_analyser.cpp libs/lexical_analyser.h libs/utils.cpp libs/utils.h
        libs/linkedList.cpp libs/linkedList.h)

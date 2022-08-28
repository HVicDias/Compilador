cmake_minimum_required(VERSION 3.23)
project(Compiler)

set(CMAKE_CXX_STANDARD 23)

add_executable(Compiler source/main.cpp libs/lexical_analiser.cpp
        libs/lexical_analiser.h libs/utils.cpp libs/utils.h )

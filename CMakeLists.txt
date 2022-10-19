cmake_minimum_required(VERSION 3.23)
project(Compiler)

set(CMAKE_CXX_STANDARD 23)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.14.0\\5.14.0\\mingw73_32\\")
#
#find_package(Qt5Widgets REQUIRED)
#
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTORCC ON)
#
#add_executable(helloworld main.cpp mainwindow.cpp application.qrc)
#target_link_libraries(helloworld Qt5::Widgets)

add_executable(Compiler source/main.cpp libs/lexicalAnalyser.cpp libs/lexicalAnalyser.h libs/utils.cpp libs/utils.h
        libs/linkedList.cpp libs/linkedList.h libs/syntacticAnalyser.cpp libs/syntacticAnalyser.h
        libs/symbolTable.cpp libs/symbolTable.h libs/semanticAnalyser.cpp libs/semanticAnalyser.h)

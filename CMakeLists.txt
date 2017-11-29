cmake_minimum_required(VERSION 3.8)
project(TP_Image_Compression)

set(CMAKE_CXX_STANDARD 11)
find_package(Imagine REQUIRED)

set(SOURCE_FILES main.cpp quadtree.h)
add_executable(TP_Image_Compression ${SOURCE_FILES})

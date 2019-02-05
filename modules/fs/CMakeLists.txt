cmake_minimum_required(VERSION 3.12)
project(fs C)

set(CMAKE_C_STANDARD 99)

add_library(fs SHARED library.c library.h)
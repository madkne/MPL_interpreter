cmake_minimum_required(VERSION 3.12)
project(modules C)

set(CMAKE_C_STANDARD 99)

add_library(modules SHARED library.c library.h)
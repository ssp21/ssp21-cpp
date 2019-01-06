cmake_minimum_required (VERSION 3.8)

project(inih)

set(inih_src
    ./inih/ini.h
    ./inih/ini.c
)

add_library(inih ${inih_src})
target_include_directories(inih PUBLIC ./inih)

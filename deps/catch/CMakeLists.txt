cmake_minimum_required (VERSION 3.8)

project(catch)

add_library(catch INTERFACE)
target_include_directories(catch INTERFACE ./catch/single_include/catch2)
target_compile_features(catch INTERFACE cxx_std_11)

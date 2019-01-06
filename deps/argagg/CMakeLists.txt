cmake_minimum_required (VERSION 3.8)

project(argagg)

add_library(argagg INTERFACE)
target_include_directories(argagg INTERFACE ./argagg/include)
target_compile_features(argagg INTERFACE cxx_std_11)

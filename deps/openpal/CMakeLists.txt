cmake_minimum_required (VERSION 3.8)

project(openpal)

file(GLOB_RECURSE openpal_src ./openpal/cpp/libs/src/openpal/*.cpp ./openpal/cpp/libs/src/openpal/*.h ./openpal/cpp/libs/include/openpal/*.h)
add_library(openpal ${openpal_src})
target_include_directories(openpal PUBLIC ./openpal/cpp/libs/include PRIVATE ./openpal/cpp/libs/src)

file(GLOB_RECURSE testlib_src ./openpal/cpp/tests/testlib/*.cpp ./openpal/cpp/tests/testlib/*.h)
add_library(testlib ${testlib_src})
target_link_libraries(testlib PUBLIC openpal)
target_include_directories(testlib PUBLIC ./openpal/cpp/tests)

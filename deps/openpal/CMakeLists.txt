cmake_minimum_required (VERSION 3.8)

project(openpal)

file(GLOB_RECURSE openpal_public_headers ./openpal/cpp/libs/include/openpal/*.h)
file(GLOB_RECURSE openpal_private_headers ./openpal/cpp/libs/src/openpal/*.h)
file(GLOB_RECURSE openpal_srcs ./openpal/cpp/libs/src/openpal/*.cpp)
add_library(openpal ${openpal_public_headers} ${openpal_private_headers} ${openpal_srcs})
target_include_directories(openpal
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/openpal/cpp/libs/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE ./openpal/cpp/libs/src
)

install(TARGETS openpal EXPORT Ssp21Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(DIRECTORY ./openpal/cpp/libs/include/openpal DESTINATION include)

file(GLOB_RECURSE testlib_src ./openpal/cpp/tests/testlib/*.cpp ./openpal/cpp/tests/testlib/*.h)
add_library(testlib ${testlib_src})
target_link_libraries(testlib PUBLIC openpal)
target_include_directories(testlib PUBLIC ./openpal/cpp/tests)

cmake_minimum_required (VERSION 3.8)

project(asio)

find_package(Threads REQUIRED)

add_library(asio INTERFACE)
target_include_directories(asio INTERFACE ./asio/asio/include)
target_compile_definitions(asio INTERFACE ASIO_STANDALONE)
target_compile_features(asio INTERFACE cxx_std_11)
target_link_libraries(asio INTERFACE Threads::Threads)

if(WIN32)
    target_link_libraries(asio INTERFACE ws2_32 wsock32) # Link to Winsock
    target_compile_definitions(asio INTERFACE _WIN32_WINNT=0x0601) # Windows 7 and up
endif()

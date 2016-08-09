cmake_minimum_required (VERSION 2.8)
project (ssp21cpp)
set(SSP21CPP_VERSION 0.0.1)

include(${PROJECT_SOURCE_DIR}/cmake/settings.cmake)

find_package(OpenSSL REQUIRED)
message("OpenSSL libraries: ${OPENSSL_LIBRARIES}")

if(WIN32)
	include_directories(${OPENSSL_INCLUDE_DIR})
endif()

set(CMAKE_REQUIRED_FLAGS ${CMAKE_CXX_FLAGS})

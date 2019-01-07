cmake_minimum_required (VERSION 3.8)

set(SSP21_VERSION 0.0.1)
project(ssp21-cpp VERSION ${SSP21_VERSION})

enable_testing()

# Dependencies
set(sodium_USE_STATIC_LIBS ON)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
find_package(sodium REQUIRED)

add_subdirectory(./deps/argagg)
add_subdirectory(./deps/asio)
add_subdirectory(./deps/catch)
add_subdirectory(./deps/inih)
add_subdirectory(./deps/openpal)

# Libs
add_subdirectory(./cpp/libs/ssp21)
add_subdirectory(./cpp/libs/sodium)
add_subdirectory(./cpp/libs/qix)

# Executables
add_subdirectory(./cpp/exe/icftool)
add_subdirectory(./cpp/exe/proxy)
add_subdirectory(./cpp/exe/qixreader)

# Tests
add_subdirectory(./cpp/tests/integration)
add_subdirectory(./cpp/tests/ssp21-afl)

# Installation
install(EXPORT Ssp21Targets
    FILE Ssp21Targets.cmake
    NAMESPACE Ssp21::
    DESTINATION lib/cmake/Ssp21
)

install(DIRECTORY ./script DESTINATION .)
install(DIRECTORY ./config DESTINATION .)

# Packaging
set(CPACK_PACKAGE_NAME ssp21)
set(CPACK_PACKAGE_VERSION_MAJOR ${ssp21-cpp_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${ssp21-cpp_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${ssp21-cpp_VERSION_PATCH})
include(CPack)

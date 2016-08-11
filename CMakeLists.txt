cmake_minimum_required (VERSION 2.8)
project (ssp21cpp)
set(SSP21CPP_VERSION 0.0.1)

include(${PROJECT_SOURCE_DIR}/cmake/settings.cmake)

# libsodium include dir
if(DEFINED SSP21_LIBSODIUM_INCLUDE_DIR)
  include_directories(${SSP21_LIBSODIUM_INCLUDE_DIR}) 
endif()

# libsodium lib dir
if(DEFINED SSP21_LIBSODIUM_LIB_DIR)
  link_directories(${SSP21_LIBSODIUM_LIB_DIR}) 
endif()


if(WIN32)
	set(SODIUM_LIB_NAME libsodium)
	# these definitions are required when linking libsodium statically on Windows
	add_definitions(-DSODIUM_STATIC=1 -DSODIUM_EXPORT=)
else()
	set(SODIUM_LIB_NAME sodium)
endif()

include_directories(./cpp/libs/include)
include_directories(./deps/openpal/cpp/libs/include)

# ---- openpal library ----
file(GLOB_RECURSE openpal_SRC ./deps/openpal/cpp/libs/src/openpal/*.cpp ./deps/openpal/cpp/libs/src/openpal/*.h ./deps/openpal/cpp/libs/include/openpal/*.h)
add_library(openpal ${LIB_TYPE} ${openpal_SRC})
install(TARGETS openpal DESTINATION lib)
set_target_properties(openpal PROPERTIES FOLDER libs)

# ---- ssp21 library ----
file(GLOB_RECURSE ssp21_SRC ./cpp/libs/src/ssp21/*.cpp ./cpp/libs/src/ssp21/*.h ./cpp/libs/include/ssp21/*.h)
add_library(ssp21 ${LIB_TYPE} ${ssp21_SRC})
target_link_libraries(ssp21 openpal)
install(TARGETS ssp21 DESTINATION lib)
set_target_properties(ssp21 PROPERTIES FOLDER libs)

# ---- sodiumbackend library ----
file(GLOB_RECURSE sodiumbackend_SRC ./cpp/libs/src/sodiumbackend/*.cpp ./cpp/libs/src/sodiumbackend/*.h ./cpp/libs/include/sodiumbackend/*.h)
add_library(sodiumbackend ${LIB_TYPE} ${sodiumbackend_SRC})
target_link_libraries(sodiumbackend ssp21 ${SODIUM_LIB_NAME})
install(TARGETS sodiumbackend DESTINATION lib)
set_target_properties(sodiumbackend PROPERTIES FOLDER libs)

enable_testing()

include_directories(./cpp/tests/catch)
include_directories(./deps/openpal/cpp/tests)

# ---- testlib ----
file(GLOB_RECURSE testlib_SRC ./deps/openpal/cpp/tests/testlib/*.cpp ./deps/openpal/cpp/tests/testlib/*.h)
add_library(testlib ${LIB_TYPE} ${testlib_SRC})
target_link_libraries(testlib openpal)
install(TARGETS testlib DESTINATION lib)
set_target_properties(testlib PROPERTIES FOLDER tests)

# ---- sodiumbackend tests ----
file(GLOB_RECURSE testsodiumbackend_SRC ./cpp/tests/sodiumbackend/*.cpp ./cpp/tests/sodiumbackend/*.h)
add_executable (testsodiumbackend ${testsodiumbackend_SRC})
target_link_libraries (testsodiumbackend sodiumbackend testlib)
set_target_properties(testsodiumbackend PROPERTIES FOLDER tests)
add_test(testsodiumbackend testsodiumbackend)

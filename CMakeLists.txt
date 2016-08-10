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
install(TARGETS ssp21 DESTINATION lib)
set_target_properties(ssp21 PROPERTIES FOLDER libs)

# ---- sodiumbackend library ----
file(GLOB_RECURSE sodiumbackend_SRC ./cpp/libs/src/sodiumbackend/*.cpp ./cpp/libs/src/sodiumbackend/*.h ./cpp/libs/include/sodiumbackend/*.h)
add_library(sodiumbackend ${LIB_TYPE} ${sodiumbackend_SRC})
install(TARGETS sodiumbackend DESTINATION lib)
set_target_properties(sodiumbackend PROPERTIES FOLDER libs)



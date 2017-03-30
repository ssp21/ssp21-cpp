cmake_minimum_required (VERSION 2.8)
project (ssp21-cpp)
set(SSP21CPP_VERSION 0.0.1)

include(${PROJECT_SOURCE_DIR}/cmake/settings.cmake)

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
target_include_directories(ssp21 PRIVATE ./cpp/libs/src)

# ---- sodiumbackend library ----
file(GLOB_RECURSE sodiumbackend_SRC ./cpp/libs/src/sodium/*.cpp ./cpp/libs/src/sodium/*.h ./cpp/libs/include/sodium/*.h)
add_library(sodiumbackend ${LIB_TYPE} ${sodiumbackend_SRC})
target_link_libraries(sodiumbackend ssp21 ${SODIUM_LIB_NAME})
install(TARGETS sodiumbackend DESTINATION lib)
set_target_properties(sodiumbackend PROPERTIES FOLDER libs)

# ---- keygen executable ----
add_executable (keygen ./cpp/exe/keygen/main.cpp)
target_link_libraries(keygen sodiumbackend)
set_target_properties(keygen PROPERTIES FOLDER exe)

enable_testing()

include_directories(./cpp/tests/catch)
include_directories(./deps/openpal/cpp/tests)

# ----- install -----

# common pattern and exludes for all installed headers
set(INSTALL_ARGS FILES_MATCHING PATTERN "*.h" PATTERN ".deps" EXCLUDE PATTERN ".libs" EXCLUDE)
install(DIRECTORY ./cpp/libs/include/ DESTINATION include ${INSTALL_ARGS})
install(DIRECTORY ./deps/openpal/cpp/libs/include/ DESTINATION include ${INSTALL_ARGS})

# ---- testlib ----
file(GLOB_RECURSE testlib_SRC ./deps/openpal/cpp/tests/testlib/*.cpp ./deps/openpal/cpp/tests/testlib/*.h)
add_library(testlib ${LIB_TYPE} ${testlib_SRC})
target_link_libraries(testlib openpal)
set_target_properties(testlib PROPERTIES FOLDER tests)

# ---- testopenpal ----
file(GLOB_RECURSE testopenpal_SRC ./deps/openpal/cpp/tests/openpaltests/src/*.cpp ./deps/openpal/cpp/tests/openpaltests/src/*.h)
add_executable (testopenpal ${testopenpal_SRC})
target_link_libraries (testopenpal testlib)
set_target_properties(testopenpal PROPERTIES FOLDER tests)
add_test(testopenpal testopenpal)

# ---- cryptobackends tests ----
file(GLOB_RECURSE cryptobackend_SRC ./cpp/tests/cryptobackend/*.cpp ./cpp/tests/cryptobackend/*.h)
add_executable (testcryptobackend ${cryptobackend_SRC})
target_link_libraries (testcryptobackend sodiumbackend testlib)
set_target_properties(testcryptobackend PROPERTIES FOLDER tests)
add_test(testcryptobackend-sodium testcryptobackend --sodium)

# ----integration tests ----
file(GLOB_RECURSE integration_SRC ./cpp/tests/integration/*.cpp ./cpp/tests/integration/*.h)
add_executable (testintegration ${integration_SRC})
target_link_libraries (testintegration ssp21 sodiumbackend testlib)
set_target_properties(testintegration PROPERTIES FOLDER tests)
add_test(testintegration-sodium testintegration --sodium)

# ---- ssp21 tests ----
file(GLOB_RECURSE testssp21_SRC ./cpp/tests/ssp21/*.cpp ./cpp/tests/ssp21/*.h)
add_executable (testssp21 ${testssp21_SRC})
target_link_libraries (testssp21 ssp21 testlib)
set_target_properties(testssp21 PROPERTIES FOLDER tests)
target_include_directories(testssp21 PRIVATE ./cpp/libs/src)
add_test(testssp21 testssp21)

add_custom_target(
	FORMAT 
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMAND astyle -R ./cpp/*.h ./cpp/*.cpp --options=./astyle.cfg --exclude=./cpp/libs/include/ssp21/crypto/gen --exclude=./cpp/libs/src/ssp21/crypto/gen --exclude=./cpp/tests/ssp21/gen
)

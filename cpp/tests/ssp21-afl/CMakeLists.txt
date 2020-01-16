set(ssp21_afl_tests_srcs
    ./main.cpp
)

add_executable(ssp21_afl_tests ${ssp21_afl_tests_srcs})
target_include_directories(ssp21_afl_tests PRIVATE ../../libs/ssp21/src)
target_link_libraries(ssp21_afl_tests PRIVATE ssp21)
clang_format(ssp21_afl_tests)

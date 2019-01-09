set(qix_tests_srcs
    ./main.cpp

    ./ParserTestSuite.cpp
)

add_executable(qix_tests ${qix_tests_srcs})
target_link_libraries(qix_tests PRIVATE qix catch)
add_test(NAME qix_tests COMMAND qix_tests)

set(sodium_backend_tests_srcs
    ./main.cpp

    ./Curve25519TestSuite.cpp
    ./SecureMemoryTestSuite.cpp
    ./SHA256TestSuite.cpp
)

add_executable(sodium_backend_tests ${sodium_backend_tests_srcs})
target_include_directories(sodium_backend_tests PRIVATE .)
target_link_libraries(sodium_backend_tests PRIVATE sodium_backend catch)
add_test(NAME sodium_backend_tests COMMAND sodium_backend_tests)

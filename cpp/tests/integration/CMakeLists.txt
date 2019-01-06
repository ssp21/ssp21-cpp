set(integration_tests_headers
    ./fixtures/IntegrationFixture.h
    ./fixtures/MockKeyStore.h

    ./mocks/LowerLayer.h
    ./mocks/SeqValidator.h
    ./mocks/UpperLayer.h
)

set(integration_tests_srcs
    ./main.cpp

    ./IntegrationTestSuite.cpp

    ./fixtures/MockKeyStore.cpp
    ./fixtures/IntegrationFixture.cpp
)

add_executable(integration_tests ${integration_tests_headers} ${integration_tests_srcs})
target_include_directories(integration_tests PRIVATE .)
target_link_libraries(integration_tests PRIVATE ssp21 sodium_backend testlib catch)
add_test(NAME integration_tests COMMAND integration_tests)

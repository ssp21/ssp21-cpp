set(ssp21_tests_headers
    ./fixtures/CryptoLayerFixture.h
    ./fixtures/LinkLayerFixture.h

    ./gen/CryptoAction.h

    ./mocks/CryptoFixture.h
    ./mocks/HexMessageBuilders.h
    ./mocks/HexSequences.h
    ./mocks/MockCertificateData.h
    ./mocks/MockCryptoBackend.h
    ./mocks/MockLowerLayer.h
    ./mocks/MockUpperLayer.h
)

set(ssp21_tests_srcs
    ./main.cpp

    ./ChainVerificationTestSuite.cpp
    ./CRCTestSuite.cpp
    ./InitiatorTestSuite.cpp
    ./LinkFormatterTestSuite.cpp
    ./LinkLayerTestSuite.cpp
    ./LinkParserTestSuite.cpp
    ./MessageParserTestSuite.cpp
    ./RequestHandshakeBeginTestSuite.cpp
    ./ResponderTestSuite.cpp
    ./SessionTestSuite.cpp
    ./VLengthTestSuite.cpp

    ./gen/CryptoAction.cpp

    ./mocks/CryptoFixture.cpp
    ./mocks/HexMessageBuilders.cpp
    ./mocks/MockCertificateData.cpp
    ./mocks/MockCryptoBackend.cpp
)

add_executable(ssp21_tests ${ssp21_tests_headers} ${ssp21_tests_srcs})
target_include_directories(ssp21_tests PRIVATE . ../src)
target_link_libraries(ssp21_tests PRIVATE ssp21 catch)
clang_format(ssp21_tests)
add_test(NAME ssp21_tests COMMAND ssp21_tests)



#include "catch.hpp"

#include "testlib/Hex.h"
#include "testlib/HexConversions.h"

#include "ssp21/crypto/gen/SessionFlags.h"
#include "ssp21/crypto/LogMessagePrinter.h"

#include "mocks/MockLogHandler.h"

#define SUITE(name) "SessionFlagsTestSuite - " name

using namespace ssp21;
using namespace openpal;

TEST_CASE(SUITE("Correct defaults"))
{
    SessionFlags flags;
    REQUIRE(flags.fir);
    REQUIRE(flags.fin);
}

TEST_CASE(SUITE("Correct serialization"))
{
    auto test_permutation = [](bool fir, bool fin, uint8_t expected)
    {
        SessionFlags flags(fir, fin);

        uint8_t value = 0;
        auto dest = wseq32_t(&value, 1);
        auto err = flags.write(dest);
        REQUIRE(!any(err));
        REQUIRE(dest.is_empty());
        REQUIRE(value == expected);
    };

    test_permutation(false, false, 0x00);
    test_permutation(true, false, 0x80);
    test_permutation(false, true, 0x40);
    test_permutation(true, true, 0xC0);
}

TEST_CASE(SUITE("Correct deserialization"))
{
    auto test_permutation = [](bool fir, bool fin, uint8_t byte)
    {
        auto input = seq32_t(&byte, 1);
        SessionFlags flags;
        REQUIRE(!any(flags.read(input)));
        REQUIRE(input.is_empty());
        REQUIRE(flags.fir == fir);
        REQUIRE(flags.fin == fin);
    };

    test_permutation(false, false, 0x00);
    test_permutation(true, false, 0x80);
    test_permutation(false, true, 0x40);
    test_permutation(true, true, 0xC0);
}

TEST_CASE(SUITE("rejects empty output"))
{
    auto output = wseq32_t::empty();
    SessionFlags flags;
    auto err = flags.write(output);
    REQUIRE(err == FormatError::insufficient_space);
}

TEST_CASE(SUITE("rejects empty input"))
{
    auto input = seq32_t::empty();
    SessionFlags flags;
    auto err = flags.read(input);
    REQUIRE(err == ParseError::insufficient_bytes);
}

TEST_CASE(SUITE("error if reserved bit is set"))
{
    uint8_t byte = 0x01;

    auto input = seq32_t(&byte, 1);
    SessionFlags flags;
    auto err = flags.read(input);
    REQUIRE(err == ParseError::reserved_bit);
}

TEST_CASE(SUITE("pretty printing"))
{
    SessionFlags flags(true, false);

    MockLogHandler log("log");
    LogMessagePrinter printer(log.logger, ssp21::levels::info, 16);

    flags.print("flags", printer);

    log.expect(
        "flags { fir: 1 fin: 0 }"
    );
}


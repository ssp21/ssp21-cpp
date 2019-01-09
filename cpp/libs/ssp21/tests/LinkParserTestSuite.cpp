

#include "catch.hpp"

#include "link/LinkParser.h"
#include "ser4cpp/util/HexConversions.h"

#define SUITE(name) "LinkParserTestSuite - " name

using namespace ssp21;
using namespace ser4cpp;

class CountingReporter : public LinkParser::IReporter
{

public:

    virtual void on_bad_header_crc(uint32_t expected, uint32_t actual) override
    {
        ++num_bad_header_crc;
    }

    virtual void on_bad_body_crc(uint32_t expected, uint32_t actual) override
    {
        ++num_bad_body_crc;
    }

    virtual void on_bad_body_length(uint32_t max_allowed, uint32_t actual) override
    {
        ++num_bad_length;
    }

    bool no_errors() const
    {
        return (num_bad_header_crc | num_bad_body_crc | num_bad_length) == 0;
    }

    void clear()
    {
        num_bad_header_crc = num_bad_body_crc = num_bad_length = 0;
    }


    uint32_t num_bad_header_crc = 0;
    uint32_t num_bad_body_crc = 0;
    uint32_t num_bad_length = 0;
};

TEST_CASE(SUITE("gracefully handles empty message"))
{
    CountingReporter reporter;
    LinkParser parser(1024, reporter);

    auto input = HexConversions::from_hex("");
    auto slice = input->as_rslice();

    REQUIRE(!parser.parse(slice));
    REQUIRE(slice.is_empty());
}

TEST_CASE(SUITE("reads a full message properly"))
{
    CountingReporter reporter;
    LinkParser parser(1024, reporter);

    auto input = HexConversions::from_hex("07 AA 00 01 00 02 00 06 11 FB E3 40 DD DD DD DD DD DD 51 0D 37 6B");
    auto slice = input->as_rslice();

    REQUIRE(parser.parse(slice));
    REQUIRE(reporter.no_errors());
    REQUIRE(slice.is_empty());

    LinkParser::Result result;

    REQUIRE(parser.read(result));
    REQUIRE(result.destination == 1);
    REQUIRE(result.source == 2);
    REQUIRE(HexConversions::to_hex(result.payload) == "DD DD DD DD DD DD");

}

TEST_CASE(SUITE("discards leading data properly"))
{
    CountingReporter reporter;
    LinkParser parser(1024, reporter);

    auto input = HexConversions::from_hex("FF FF FF FF 07 AA 00 01 00 02 00 06 11 FB E3 40 DD DD DD DD DD DD 51 0D 37 6B");
    auto slice = input->as_rslice();

    REQUIRE(parser.parse(slice));
    REQUIRE(reporter.no_errors());
    REQUIRE(slice.is_empty());
}

TEST_CASE(SUITE("detects header crc failure properly"))
{
    CountingReporter reporter;
    LinkParser parser(1024, reporter);

    ///                                   -------------------------VV----------------------------------------
    auto input = HexConversions::from_hex("07 AA 00 01 00 02 00 06 12 FB E3 40 DD DD DD DD DD DD 51 0D 37 6B");
    auto slice = input->as_rslice();

    REQUIRE_FALSE(parser.parse(slice));
    REQUIRE(reporter.num_bad_header_crc == 1);
    REQUIRE(slice.is_empty());
}

TEST_CASE(SUITE("recursively processes header crc failure"))
{
    CountingReporter reporter;
    LinkParser parser(1024, reporter);

    {
        // packet in a packet
        //                                           07 AA 00 01 00 02 00 06 11 FB E3 40 DD DD DD DD DD DD 51 0D 37 6B < - valid packet
        auto input = HexConversions::from_hex("07 AA 07 AA 00 01 00 02 00 06 11 FB E3 40 DD DD DD DD DD DD 51");
        auto slice = input->as_rslice();

        REQUIRE_FALSE(parser.parse(slice));
        REQUIRE(reporter.num_bad_header_crc == 1);
        REQUIRE(slice.is_empty());
    }

    reporter.clear();

    {
        // last 3 bytes of body CRC will complete the frame
        auto input = HexConversions::from_hex("0D 37 6B");
        auto slice = input->as_rslice();

        REQUIRE(parser.parse(slice));
        REQUIRE(reporter.no_errors());
        REQUIRE(slice.is_empty());
    }
}

TEST_CASE(SUITE("detects body crc failure properly"))
{
    CountingReporter reporter;
    LinkParser parser(1024, reporter);

    ///                                   ----------------------------------------------------------VV-------
    auto input = HexConversions::from_hex("07 AA 00 01 00 02 00 06 11 FB E3 40 DD DD DD DD DD DD 51 0E 37 6B");
    auto slice = input->as_rslice();

    REQUIRE_FALSE(parser.parse(slice));
    REQUIRE(reporter.num_bad_body_crc == 1);
    REQUIRE(slice.is_empty());
}

TEST_CASE(SUITE("allows frame with maximum body length"))
{
    CountingReporter reporter;
    LinkParser parser(6, reporter);

    ///                                  --------------------LL-LL-------------------------------------------
    auto input = HexConversions::from_hex("07 AA 00 01 00 02 00 06 11 FB E3 40 DD DD DD DD DD DD 51 0D 37 6B");
    auto slice = input->as_rslice();

    REQUIRE(parser.parse(slice));
    REQUIRE(reporter.no_errors());
    REQUIRE(slice.is_empty());
}

TEST_CASE(SUITE("fails if length exceeds maximum body length"))
{
    CountingReporter reporter;
    LinkParser parser(5, reporter);

    ///                                   -------------------LL-LL-------------------------------------------
    auto input = HexConversions::from_hex("07 AA 00 01 00 02 00 06 11 FB E3 40 DD DD DD DD DD DD 51 0D 37 6B");
    auto slice = input->as_rslice();

    REQUIRE_FALSE(parser.parse(slice));
    REQUIRE(reporter.num_bad_length == 1);
    REQUIRE(reporter.num_bad_header_crc == 0);
    REQUIRE(reporter.num_bad_body_crc == 0);
    REQUIRE(slice.is_empty());
}

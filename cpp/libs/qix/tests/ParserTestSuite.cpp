

#include "catch.hpp"

#include "log4cpp/MockLogHandler.h"
#include "qix/QIXFrameParser.h"
#include "ser4cpp/util/HexConversions.h"

#define SUITE(name) "ParserTestSuite - " name

using namespace ssp21;
using namespace ser4cpp;

TEST_CASE(SUITE("correctly parses QIX frame"))
{
    log4cpp::MockLogHandler handler("test");
    QIXFrameParser parser(handler.logger);

    const std::string sync("5A A5");
    const std::string key_id("00 00 00 00 00 00 00 01");
    const std::string key_data("00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F");
    const std::string key_status("01");
    const std::string crc("0F AE 44 5E");

    auto hex = HexConversions::from_hex(sync + key_id + key_data + key_status + crc);
    REQUIRE(hex->as_rslice().length() == QIXFrameParser::get_fixed_frame_size());

    auto dest = parser.get_write_slice();
    REQUIRE(dest.length() == QIXFrameParser::get_fixed_frame_size());
    dest.copy_from(hex->as_rslice());

    QIXFrame frame;
    REQUIRE(parser.parse(frame));

    REQUIRE(frame.key_id == 1);
    REQUIRE(HexConversions::to_hex(frame.key_data) == key_data);
    REQUIRE(frame.status == QIXFrame::Status::ok);
}

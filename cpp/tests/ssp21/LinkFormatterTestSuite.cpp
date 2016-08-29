

#include "catch.hpp"

#include "ssp21/link/LinkFormatter.h"
#include "ssp21/link/LinkConstants.h"

#include "testlib/Hex.h"
#include "testlib/HexConversions.h"

#include "openpal/container/StaticBuffer.h"

#define SUITE(name) "LinkFormatterTestSuite - " name

using namespace ssp21;
using namespace openpal;

TEST_CASE(SUITE("correctly formats output"))
{
    StaticBuffer<100> buffer;

    Hex payload("DD DD DD DD DD DD");

    auto result = LinkFormatter::write(buffer.as_wslice(), Message(Addresses(1, 2), payload));
    REQUIRE(result.length() == consts::min_link_frame_size + 6);
    REQUIRE(to_hex(result) == "07 AA 00 01 00 02 00 06 11 FB E3 40 DD DD DD DD DD DD 51 0D 37 6B");
}

TEST_CASE(SUITE("returns empty buffer if less space than minimum frame size"))
{
    StaticBuffer < consts::min_link_frame_size - 1 > buffer;

    auto result = LinkFormatter::write(buffer.as_wslice(), Message(Addresses(1, 2), RSlice::empty_slice()));

    REQUIRE(result.is_empty());
}

TEST_CASE(SUITE("returns empty buffer if insufficient space for payload"))
{
    StaticBuffer < consts::min_link_frame_size + 5 > buffer;

    Hex payload("DD DD DD DD DD DD");

    auto result = LinkFormatter::write(buffer.as_wslice(), Message(Addresses(1, 2), payload));

    REQUIRE(result.is_empty());
}

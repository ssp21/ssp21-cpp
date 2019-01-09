

#include "catch.hpp"

#include "fixtures/LinkLayerFixture.h"
#include "mocks/HexMessageBuilders.h"

#define SUITE(name) "LinkLayerTestSuite - " name

using namespace ssp21;
using namespace ser4cpp;

TEST_CASE(SUITE("forwards open/close"))
{
    LinkLayerFixture fix;

    REQUIRE_FALSE(fix.upper.get_is_open());
    fix.link.on_lower_open();
    REQUIRE(fix.upper.get_is_open());
    fix.link.on_lower_close();
    REQUIRE_FALSE(fix.upper.get_is_open());
}

TEST_CASE(SUITE("processes a valid frame"))
{
    LinkLayerFixture fix;
    fix.link.on_lower_open();
    fix.lower.enqueue_message(hex::link_frame(10, 1, "CA FE"));
    REQUIRE(fix.upper.pop_rx_message() == "CA FE");
}

TEST_CASE(SUITE("processes partial frames"))
{
    LinkLayerFixture fix;
    fix.link.on_lower_open();

    const auto split_at = 3 * consts::link::header_total_size; // split the frame after the header

    const auto full_message = hex::link_frame(10, 1, "CA FE");
    const auto start_message = full_message.substr(0, split_at);
    const auto end_message = full_message.substr(split_at, full_message.size() - split_at);

    fix.lower.enqueue_message(start_message);
    REQUIRE(fix.upper.is_empty());
    fix.lower.enqueue_message(end_message);
    REQUIRE(fix.upper.pop_rx_message() == "CA FE");
}

TEST_CASE(SUITE("processes multiple frames received in a single chunk"))
{
    LinkLayerFixture fix;
    fix.link.on_lower_open();

    const auto message1 = hex::link_frame(10, 1, "CA FE");
    const auto message2 = hex::link_frame(10, 1, "BA BE");

    fix.lower.enqueue_message(message1 + message2);

    REQUIRE(fix.upper.pop_rx_message() == "CA FE");
    REQUIRE(fix.upper.pop_rx_message() == "BA BE");
}

TEST_CASE(SUITE("forwards transmitted data"))
{
    LinkLayerFixture fix;
    fix.link.on_lower_open();

    auto message = HexConversions::from_hex("CA FE");
    IUpperLayer& link_upper = fix.link;
    ILowerLayer& link_lower = fix.link;

    for (int i = 0; i < 3; ++i)
    {
        REQUIRE(fix.upper.num_tx_ready == i);
        REQUIRE(link_lower.start_tx_from_upper(message->as_rslice()));
        REQUIRE(fix.lower.pop_tx_message() == "CA FE");
        REQUIRE(fix.lower.num_tx_messages() == 0);

        REQUIRE(link_upper.on_lower_tx_ready());
        REQUIRE(fix.upper.num_tx_ready == i + 1);
    }
}



#include "catch.hpp"

#include "link/LinkFrameWriter.h"

#include "ser4cpp/container/StaticBuffer.h"
#include "ser4cpp/util/HexConversions.h"

#define SUITE(name) "LinkFormatterTestSuite - " name

using namespace ssp21;
using namespace ser4cpp;

class HexWritable : public ssp21::IWritable
{

public:

    HexWritable(const std::string& payload)
        : payload(HexConversions::from_hex(payload))
    {}

    virtual FormatResult write(wseq32_t& output) const override
    {
        const auto src = payload->as_rslice();
        return src.length() < output.length() ? FormatResult::success(output.copy_from(src)) : FormatResult::error(FormatError::insufficient_space);
    }

    virtual void print(IMessagePrinter& printer) const override {}

    virtual Function get_function() const
    {
        return Function::undefined;
    }

private:

    std::unique_ptr<ser4cpp::Buffer> payload;
};

TEST_CASE(SUITE("correctly formats output"))
{
    LinkFrameWriter writer(openpal::Logger::empty(), Addresses(1, 2), 6);

    HexWritable payload("DD DD DD DD DD DD");
    auto result = writer.write(payload);

    REQUIRE_FALSE(result.is_error());
    REQUIRE(HexConversions::to_hex(result.written) == "DD DD DD DD DD DD");
    REQUIRE(HexConversions::to_hex(result.frame) == "07 AA 00 01 00 02 00 06 11 FB E3 40 DD DD DD DD DD DD 51 0D 37 6B");
}

TEST_CASE(SUITE("fails if insufficient space to write payload"))
{
    LinkFrameWriter writer(openpal::Logger::empty(), Addresses(1, 2), 5);

    HexWritable payload("DD DD DD DD DD DD");
    auto result = writer.write(payload);

    REQUIRE(result.is_error());
    REQUIRE(result.err == FormatError::insufficient_space);
    REQUIRE(result.written.is_empty());
    REQUIRE(result.frame.is_empty());
}

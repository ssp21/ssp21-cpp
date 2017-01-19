

#include "catch.hpp"

#include "ssp21/link/LinkFrameWriter.h"
#include "ssp21/crypto/SeqField.h"

#include "testlib/Hex.h"
#include "testlib/HexConversions.h"


#include "openpal/container/StaticBuffer.h"

#define SUITE(name) "LinkFormatterTestSuite - " name

using namespace ssp21;
using namespace openpal;

class HexWritable : public ssp21::IWritable
{

public:

    HexWritable(const std::string& payload) : payload(payload)
    {}

    virtual FormatResult write(wseq32_t& output) const override
    {
        const auto src = payload.as_rslice();
        return src.length() < output.length() ? FormatResult::success(output.copy_from(src)) : FormatResult::error(FormatError::insufficient_space);
    }

    virtual void print(IMessagePrinter& printer) const override {}

    virtual Function get_function() const
    {
        return Function::undefined;
    }

private:

    Hex payload;
};

TEST_CASE(SUITE("correctly formats output"))
{
    LinkFrameWriter writer(openpal::Logger::empty(), Addresses(1, 2), 6);

    HexWritable payload("DD DD DD DD DD DD");
    auto result = writer.write(payload);

    REQUIRE_FALSE(result.is_error());
    REQUIRE(to_hex(result.written) == "DD DD DD DD DD DD");
    REQUIRE(to_hex(result.frame) == "07 AA 00 01 00 02 00 06 11 FB E3 40 DD DD DD DD DD DD 51 0D 37 6B");
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

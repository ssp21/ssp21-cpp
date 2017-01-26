
#include "ssp21/link/LinkFrameReader.h"

namespace ssp21
{

    LinkFrameReader::LinkFrameReader(const openpal::Logger& logger, uint16_t address, uint16_t max_payload_length) :
        logger(logger),
        address(address),
        parser(max_payload_length, *this)
    {}

    seq32_t LinkFrameReader::read(seq32_t& input)
    {
        if (this->parser.parse(input))
        {
            LinkParser::Result res;
            this->parser.read(res);

            // TODO - check the address

            return res.payload;
        }
        else
        {
            return seq32_t::empty();
        }
    }

    void LinkFrameReader::reset()
    {
        parser.reset();
    }

}



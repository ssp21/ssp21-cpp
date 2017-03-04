
#ifndef SSP21_LINKFRAMEREADER_H
#define SSP21_LINKFRAMEREADER_H

#include "ssp21/IFrameReader.h"

#include "openpal/logging/Logger.h"
#include "ssp21/link/LinkParser.h"

namespace ssp21
{

    class LinkFrameReader final : public IFrameReader, private LinkParser::IReporter
    {

    public:

        LinkFrameReader(const openpal::Logger& logger, uint16_t address, uint16_t max_payload_length);

        virtual seq32_t read(seq32_t& input) override;

        virtual void reset() override;

    private:

        virtual void on_bad_header_crc(uint32_t expected, uint32_t actual) override {}
        virtual void on_bad_body_crc(uint32_t expected, uint32_t actual) override {}
        virtual void on_bad_body_length(uint32_t max_allowed, uint32_t actual) override {}

        openpal::Logger logger;
        uint16_t address;
        LinkParser parser;

    };

}

#endif

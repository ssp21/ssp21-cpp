
#ifndef SSP21_RAWFRAMEWRITER_H
#define SSP21_RAWFRAMEWRITER_H

#include "IFrameWriter.h"

#include "ser4cpp/container/Buffer.h"

#include "ssp21/link/LinkConstants.h"
#include "ssp21/link/Addresses.h"

namespace ssp21
{

    class RawFrameWriter final : public IFrameWriter
    {

    public:

        RawFrameWriter(const log4cpp::Logger& logger, uint16_t max_payload_size);

        virtual uint16_t get_max_payload_size() const override
        {
            return max_payload_size;
        }

    private:

        virtual WriteResult write_impl(const IWritable& payload) override;

        uint16_t max_payload_size;
        ser4cpp::Buffer frame_buffer;

    };

}

#endif

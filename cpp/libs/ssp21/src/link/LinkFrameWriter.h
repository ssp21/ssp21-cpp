
#ifndef SSP21_LINKFRAMEWRITER_H
#define SSP21_LINKFRAMEWRITER_H

#include "IFrameWriter.h"

#include "ser4cpp/container/Buffer.h"

#include "ssp21/link/Addresses.h"
#include "ssp21/link/LinkConstants.h"

namespace ssp21 {

class LinkFrameWriter final : public IFrameWriter {

public:
    LinkFrameWriter(const log4cpp::Logger& logger, Addresses addr, uint16_t max_payload_size);

    virtual uint16_t get_max_payload_size() const override
    {
        return max_payload_size;
    }

private:
    virtual WriteResult write_impl(const IWritable& payload) override;

    FormatResult write_body_and_crc(const IWritable& payload);

    static constexpr uint32_t get_buffer_size(uint16_t max_payload_size)
    {
        return static_cast<uint32_t>(consts::link::min_frame_size) + static_cast<uint32_t>(max_payload_size);
    }

    seq32_t last_frame;

    Addresses addr;
    uint16_t max_payload_size;
    ser4cpp::Buffer frame_buffer;
};

}

#endif

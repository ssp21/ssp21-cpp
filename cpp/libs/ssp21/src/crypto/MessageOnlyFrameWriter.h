

#ifndef SSP21_MESSAGEONLYFRAMEWRITER_H
#define SSP21_MESSAGEONLYFRAMEWRITER_H

#include "IFrameWriter.h"
#include "ser4cpp/container/Buffer.h"
#include "ssp21/link/LinkConstants.h"

namespace ssp21 {

class MessageOnlyFrameWriter final : public IFrameWriter {
public:
    MessageOnlyFrameWriter(const log4cpp::Logger& logger = log4cpp::Logger::empty(), uint16_t max_payload_size = consts::link::max_config_payload_size)
        : IFrameWriter(logger)
        , max_payload_size(max_payload_size)
        , buffer(max_payload_size)
    {
    }

    virtual uint16_t get_max_payload_size() const override
    {
        return max_payload_size;
    }

private:
    virtual WriteResult write_impl(const IWritable& payload) override
    {
        auto dest = buffer.as_wslice();
        const auto res = payload.write(dest);
        if (res.is_error())
            return WriteResult::error(res.err);
        else {
            return WriteResult::success(res, res.written);
        }
    }

    uint16_t max_payload_size;
    ser4cpp::Buffer buffer;
};

}

#endif

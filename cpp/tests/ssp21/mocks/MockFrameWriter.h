

#ifndef RESPONDER_MOCKFRAMEWRITER_H
#define RESPONDER_MOCKFRAMEWRITER_H

#include "ssp21/IFrameWriter.h"
#include "openpal/container/Buffer.h"
#include "ssp21/link/LinkConstants.h"

namespace ssp21
{

   
class MockFrameWriter : public IFrameWriter
{
public:

    explicit MockFrameWriter(uint16_t max_payload_size = consts::link::max_config_payload_size) : max_payload_size(max_payload_size), buffer(max_payload_size) {}

    virtual WriteResult write(const IWritable& payload)  override
    {
        auto dest = buffer.as_wslice();
        const auto res = payload.write(dest);
        if (res.is_error()) return WriteResult::error(res.err);
        else
        {
            return WriteResult::success(res, res.written);
        }
    }

    virtual uint16_t get_max_payload_size() const override
    {
        return max_payload_size;
    }

private:

    uint16_t max_payload_size;
    openpal::Buffer buffer;
};


}

#endif


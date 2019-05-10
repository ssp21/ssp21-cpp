
#include "RawFrameWriter.h"

#include "ser4cpp/serialization/BigEndian.h"

namespace ssp21
{

    RawFrameWriter::RawFrameWriter(const log4cpp::Logger& logger, uint16_t max_payload_size) :
        IFrameWriter(logger),
        max_payload_size(max_payload_size),
        frame_buffer(max_payload_size)
    {}

    WriteResult RawFrameWriter::write_impl(const IWritable& payload)
    {
        auto dest = frame_buffer.as_wslice();
        const auto res = payload.write(dest);

        if (res.is_error())
        {
            return WriteResult::error(res.err);
        }

        return WriteResult::success(res, this->frame_buffer.as_rslice().take(res.written.length()));
    }

}



#include "link/LinkFrameWriter.h"

#include "ssp21/link/CastagnoliCRC32.h"
#include "ser4cpp/serialization/BigEndian.h"

namespace ssp21
{

    LinkFrameWriter::LinkFrameWriter(const log4cpp::Logger& logger, Addresses addr, uint16_t max_payload_size) :
        IFrameWriter(logger),
        addr(addr),
        max_payload_size(max_payload_size),
        frame_buffer(get_buffer_size(max_payload_size))
    {}

    WriteResult LinkFrameWriter::write_impl(const IWritable& payload)
    {
        const auto res = this->write_body_and_crc(payload);
        if (res.is_error()) return WriteResult::error(res.err);

        // now that we've written the body, we can write the header

        if (res.written.length() > std::numeric_limits<uint16_t>::max())
        {
            return WriteResult::error(FormatError::insufficient_space);
        }

        const uint16_t payload_length = static_cast<uint16_t>(res.written.length());

        auto dest = this->frame_buffer.as_wslice();

        if (!ser4cpp::BigEndian::write(
                    dest,
                    consts::link::sync1,
                    consts::link::sync2,
                    addr.destination,
                    addr.source,
                    payload_length))
        {
            return WriteResult::error(FormatError::insufficient_space);
        }

        const auto crc_h = CastagnoliCRC32::calc(this->frame_buffer.as_rslice().take(consts::link::header_fields_size));

        if (!ser4cpp::UInt32::write_to(dest, crc_h))
        {
            return WriteResult::error(FormatError::insufficient_space);
        }

        return WriteResult::success(res, this->frame_buffer.as_rslice().take(res.written.length() + consts::link::min_frame_size));
    }

    FormatResult LinkFrameWriter::write_body_and_crc(const IWritable& payload)
    {
        auto dest = frame_buffer.as_wslice();
        dest.advance(consts::link::header_total_size);

        const auto result = payload.write(dest);
        if (result.is_error()) return result;

        if (!ser4cpp::BigEndian::write(dest, CastagnoliCRC32::calc(result.written)))
        {
            return FormatResult::error(FormatError::insufficient_space);
        }

        return result;
    }

}


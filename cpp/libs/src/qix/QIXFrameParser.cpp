#include "qix/QIXFrameParser.h"

#include "ssp21/link/CastagnoliCRC32.h"
#include "ssp21/stack/LogLevels.h"

#include "openpal/serialization/BigEndian.h"

#include "openpal/logging/LogMacros.h"

using namespace ssp21;

ssp21::wseq32_t QIXFrameParser::get_write_slice()
{
    auto dest = this->buffer.as_wseq();
    dest.advance(this->num_bytes_buffered);
    return dest;
}

bool QIXFrameParser::parse(QIXFrame& frame)
{
    this->num_bytes_buffered = 0;
    auto remainder = this->buffer.as_seq();

    // first, consume any input until we hit the first sync character
    this->find_sync(remainder);

    if (remainder.length() < total_frame_size)
    {
        // had to discard some bytes, so we need to read some more bytes
        this->num_bytes_buffered = remainder.length();
        this->buffer.as_wseq().copy_from(remainder);
        return false;
    }

    const auto calc_crc = this->calc_frame_crc();
    const auto actual_crc = this->read_frame_crc();

    // we can now parse the frame. first verify the CRC.
    if (calc_crc != actual_crc)
    {
        FORMAT_LOG_BLOCK(this->logger, levels::warn, "CRC on frame (0x%x) doesn't match calculated crc (0x%x)", actual_crc, calc_crc);
        // toss the entire frame
        return false;
    }

    this->read_frame_fields(frame);
    return true;
}

void QIXFrameParser::read_frame_fields(QIXFrame& frame)
{
    auto payload = this->buffer.as_seq().skip(2);
    // read the key_id, and advance the slice
    openpal::BigEndian::read(payload, frame.key_id);
    // the next 32 bytes are the actual key
    frame.key_data = payload.take(32);
}

uint32_t QIXFrameParser::calc_frame_crc()
{
    return CastagnoliCRC32::calc(this->buffer.as_seq().skip(2).take(40));
}

uint32_t QIXFrameParser::read_frame_crc()
{
    auto data = this->buffer.as_seq().skip(42);
    uint32_t crc;
    openpal::BigEndian::read(data, crc);
    return crc;
}

bool QIXFrameParser::find_sync(ssp21::seq32_t& input)
{
    // search the first two bytes for a match
    while (input.length() > 0)
    {
        if (input.length() == 1)
        {
            if (input[0] == QIXFrameParser::sync1)
            {
                return true;
            }
        }
        else
        {
            if (input[0] == QIXFrameParser::sync1 && input[1] == QIXFrameParser::sync2)
            {
                return true;
            }
        }

        input.advance(1);
    }

    return false;
}

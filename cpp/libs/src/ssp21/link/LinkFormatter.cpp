
#include "ssp21/link/LinkFormatter.h"

#include "ssp21/link/LinkConstants.h"
#include "ssp21/link/CastagnoliCRC32.h"

#include "openpal/util/Comparisons.h"
#include "openpal/serialization/BigEndian.h"

using namespace openpal;

namespace ssp21
{
    seq32_t LinkFormatter::write(wseq32_t dest, const Addresses& addr, const seq32_t& payload)
    {
        const auto start = dest.readonly();

        if (dest.length() < consts::link::min_frame_size)
        {
            return seq32_t::empty();
        }

        const uint32_t max_payload_length = min<uint32_t>(
                                                // don't let the user write anything bigger than configured maximum
                                                consts::link::max_config_payload_size,
                                                // maximum determined by the output buffer
                                                dest.length() - consts::link::min_frame_size
                                            );

        if (payload.length() > max_payload_length)
        {
            return seq32_t::empty();
        }

        // safe cast since we've already validated the payload length relative to the maximum allowed
        const auto payload_length = static_cast<uint16_t>(payload.length());

        BigEndian::write(
            dest,
            consts::link::sync1,
            consts::link::sync2,
            addr.destination,
            addr.source,
            payload_length
        );

        const auto crc_h = CastagnoliCRC32::calc(start.take<uint32_t>(consts::link::header_fields_size));

        UInt32::write_to(dest, crc_h);

        // copy the payload
        dest.copy_from(payload);

        // append the body crc
        UInt32::write_to(dest, CastagnoliCRC32::calc(payload));

        return start.take<uint32_t>(consts::link::min_frame_size + payload_length);
    }
}


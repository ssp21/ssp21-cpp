
#include "ssp21/link/LinkFormatter.h"

#include "ssp21/link/LinkConstants.h"
#include "ssp21/link/CastagnoliCRC32.h"

#include "openpal/util/Comparisons.h"
#include "openpal/serialization/BigEndian.h"



using namespace openpal;

namespace ssp21
{
	RSlice LinkFormatter::write(WSlice dest, const Message& message)
    {
        if (dest.length() < consts::min_link_frame_size)
        {
            return RSlice::empty_slice();
        }

        const uint32_t max_payload_length = min<uint32_t>(
                                                // don't let the user write anything bigger than configured maximum
                                                consts::max_config_link_payload_size,
                                                // maximum determined by the output buffer
                                                dest.length() - consts::min_link_frame_size
                                            );

        if (message.payload.length() > max_payload_length)
        {
            return RSlice::empty_slice();
        }

        // safe cast since we've already validated the payload length relative to the maximum allowed
		const auto payload_length = static_cast<uint16_t>(message.payload.length());

        // there's enough space to write the entire frame
        // const auto frame_size = consts::min_link_frame_size + payload.length();

        dest[0] = consts::sync1;
        dest[1] = consts::sync2;

        UInt16::write(dest.skip(2), message.addresses.destination);
		UInt16::write(dest.skip(4), message.addresses.source);
        UInt16::write(dest.skip(6), payload_length);

        // append the header crc
        const auto crc_h = CastagnoliCRC32::calc(dest.as_rslice().take(consts::link_header_fields_size));
        UInt32::write(dest.skip(consts::link_header_fields_size), crc_h);

        // copy the payload
        auto payload_start = dest.skip(consts::link_header_total_size);
		message.payload.copy_to(payload_start);

        // append the body crc
        UInt32::write(dest.skip(consts::link_header_total_size + payload_length), CastagnoliCRC32::calc(message.payload));

        return dest.as_rslice().take(consts::min_link_frame_size + payload_length);
    }
}


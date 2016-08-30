
#ifndef SSP21_LINKCONSTANTS_H
#define SSP21_LINKCONSTANTS_H

#include <cstdint>

namespace ssp21
{
    namespace consts
    {
		namespace link
		{
			// stuff for the link parser
			const uint8_t sync1 = 0x07;
			const uint8_t sync2 = 0xAA;
			const uint16_t max_config_payload_size = 4092;
			const uint16_t crc_size = 4;
			const uint16_t header_fields_size = 8;
			const uint16_t header_total_size = header_fields_size + crc_size;
			const uint16_t min_frame_size = header_total_size + crc_size;

			//defaults
			const uint16_t default_responder_local_address = 1;
			const uint16_t default_responder_remote_address = 10;
		}
    }
}

#endif

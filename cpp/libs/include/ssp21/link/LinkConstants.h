
#ifndef SSP21_LINKCONSTANTS_H
#define SSP21_LINKCONSTANTS_H

#include <cstdint>

namespace ssp21
{
    namespace consts
    {
        const uint8_t sync1 = 0x07;
        const uint8_t sync2 = 0xAA;
        const uint16_t max_config_link_payload_size = 4092;
        const uint16_t crc_size = 4;
        const uint16_t link_header_fields_size = 8;
        const uint16_t link_header_total_size = link_header_fields_size + crc_size;
        const uint16_t min_link_frame_size = link_header_total_size + crc_size;
    }
}

#endif

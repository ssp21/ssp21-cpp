#ifndef SSP21_LINKCONSTANTS_H
#define SSP21_LINKCONSTANTS_H

#include <cstdint>

namespace ssp21
{
    /**
     * @brief Useful constants
     */
    namespace consts
    {
        /**
         * @brief Link-layer constants.
         */
        namespace link
        {
            /**
             * Link-layer first sync byte value.
             */
            const uint8_t sync1 = 0x07;
            /**
             * Link-layer second sync byte value.
             */
            const uint8_t sync2 = 0xAA;
            /**
             * Link-layer maximum payload size.
             */
            const uint16_t max_config_payload_size = 4092;
            /**
             * Length of CRCs.
             */
            const uint16_t crc_size = 4;
            /**
             * Link-layer header length (with sync bytes, without CRC).
             */
            const uint16_t header_fields_size = 8;
            /**
             * Link-layer header total length.
             */
            const uint16_t header_total_size = header_fields_size + crc_size;
            /**
             * Link-layer minimum frame length.
             */
            const uint16_t min_frame_size = header_total_size + crc_size;
            /**
             * Link-layer maximum frame length.
             */
            const uint16_t max_frame_size = min_frame_size + max_config_payload_size;

            /**
             * Default link-layer local address.
             */
            const uint16_t default_responder_local_address = 1;
            /**
             * Default link-layer remote address.
             */
            const uint16_t default_responder_remote_address = 10;
        }
    }
}

#endif

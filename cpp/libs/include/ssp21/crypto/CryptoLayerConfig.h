
#ifndef SSP21_CRYPTOLAYERCONFIG_H
#define SSP21_CRYPTOLAYERCONFIG_H

#include <cstdint>

#include "ssp21/crypto/Constants.h"

namespace ssp21
{

    struct SessionConfig
    {
        SessionConfig() {}

        explicit SessionConfig(uint32_t ttl_pad_ms) : ttl_pad_ms(ttl_pad_ms) {}

        // the TTL padding added to the current session time of every message
        uint32_t ttl_pad_ms = consts::crypto::default_ttl_pad_ms;
    };

    struct CryptoLayerConfig
    {
        // The maximum size of the payload data
        uint16_t max_payload_size = consts::link::max_config_payload_size;
    };

    struct ResponderConfig
    {
        CryptoLayerConfig config;
        SessionConfig session;
    };

    struct InitiatorConfig
    {        
        // TODO - Add a copy constructor that applies reasonable limits/relationships to some of these values
        struct Params
        {
            /// How long the initiator will will for responses
            openpal::TimeDuration response_timeout = consts::crypto::initiator::default_response_timeout;

            /// How long the initiator will wait before retrying a failed timeout
            openpal::TimeDuration retry_timeout = consts::crypto::initiator::default_retry_timeout;

            /// The maximum session time that the initiator will request in the handshake
            uint32_t max_session_time_ms = consts::crypto::initiator::default_max_session_time_ms;

            /// The initiator will begin renegotiating when the session time reaches this value
            uint32_t session_time_renegotiation_trigger_ms = consts::crypto::initiator::default_session_time_renegotiation_trigger_ms;

            /// The maximum tx or rx nonce value that the initiator will request in the handshake
            uint16_t max_nonce_value = consts::crypto::initiator::default_max_nonce;

            /// The initiator will begin renegotiating when either nonce value reaches this trigger level
            uint16_t nonce_renegotiation_trigger_value = consts::crypto::initiator::default_nonce_renegotiation_trigger;
        };
        
        Params params;
        CryptoLayerConfig config;
        SessionConfig session;
    };

}

#endif

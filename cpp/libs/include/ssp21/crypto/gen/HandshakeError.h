//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// 
// This file is auto-generated. Do not edit manually
// 
// Licensed under the terms of the BSDv3 license
//

#ifndef SSP21_HANDSHAKEERROR_H
#define SSP21_HANDSHAKEERROR_H

#include <cstdint>
#include "openpal/util/Uncopyable.h"

namespace ssp21 {

/**
    Denotes an error condition that occurred during the handshake process
*/
enum class HandshakeError : uint8_t
{
    /// A received handshake message was malformed in some manner
    bad_message_format = 0x0,
    /// The requested version is not supported
    unsupported_version = 0x1,
    /// The requested handshake ephemeral is not supported or doesn't match the handshake mode
    unsupported_handshake_ephemeral = 0x2,
    /// The requested handshake hash is not supported
    unsupported_handshake_hash = 0x3,
    /// The requested handshake KDF is not supported
    unsupported_handshake_kdf = 0x4,
    /// The requested session mode is not supported
    unsupported_session_mode = 0x6,
    /// The requested nonce verification mode is not supported
    unsupported_nonce_mode = 0x7,
    /// The requested handshake mode is not supported
    unsupported_handshake_mode = 0x8,
    /// One of the received certificates was improperly encoded
    bad_certificate_format = 0x9,
    /// One of the received certificates was improperly encoded
    bad_certificate_chain = 0xA,
    /// One of the received certificates utilizes an unsupported feature
    unsupported_certificate_feature = 0xB,
    /// The outstation was unable to authenticate the master
    authentication_error = 0xC,
    /// The master requested handshake auth, but no prior handshake begin was received
    no_prior_handshake_begin = 0xD,
    /// This value gets used internally in ssp21-cpp only
    none = 0xFD,
    /// value not defined
    undefined = 0xFE,
    /// internal error
    internal = 0xFF
};

inline bool any(HandshakeError value)
{
    return value != HandshakeError::none;
}

struct HandshakeErrorSpec : private openpal::StaticOnly
{
    typedef HandshakeError enum_type_t;

    static uint8_t to_type(HandshakeError arg);
    static HandshakeError from_type(uint8_t arg);
    static const char* to_string(HandshakeError arg);
};

}

#endif

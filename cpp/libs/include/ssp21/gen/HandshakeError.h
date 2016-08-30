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
// License TBD
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
    /// The requested DH mode is not supported
    unsupported_dh_mode = 0x2,
    /// The requested hash mode is not supported
    unsupported_hash_mode = 0x3,
    /// The requested session mode is not supported
    unsupported_session_mode = 0x4,
    /// The requested nonce verification mode is not supported
    unsupported_nonce_mode = 0x5,
    /// The requested certificate mode is not supported
    unsupported_certificate_mode = 0x6,
    /// One of the received certificates was improperly encoded
    bad_certificate_format = 0x7,
    /// One of the received certificates utilizes an unsupported feature
    unsupported_certificate_feature = 0x8,
    /// The outstation was unable to authenticate the master
    authentication_error = 0x9,
    /// internal error
    internal = 0xA,
    /// This value gets used internally in ssp21-cpp only
    none = 0xFE,
    /// value not defined
    undefined = 0xFF
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

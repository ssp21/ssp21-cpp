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

#ifndef SSP21_CRYPTOERROR_H
#define SSP21_CRYPTOERROR_H

#include <cstdint>
#include "openpal/util/Uncopyable.h"
#include "ssp21/util/ErrorCategory.h"

namespace ssp21 {

/**
    Various errors in the cryptographic layer
*/
enum class CryptoError : uint8_t
{
    /// no error
    ok = 0x0,
    /// attempted operation with bad key type
    bad_key_type = 0x1,
    /// X25519 DH operation failed. Bad public key?
    dh_x25519_fail = 0x2,
    /// a procedure cannot be executed b/c of an undefined algorithm
    undefined_algorithm = 0x3,
    /// A calculated MAC did not match the specified MAC
    mac_auth_fail = 0x4,
    /// A buffer had an insufficient or incorrect size
    bad_buffer_size = 0x5,
    /// No validate session for authenticating user data
    no_valid_session = 0x6,
    /// The TTL on a message is expired
    expired_ttl = 0x7,
    /// The nonce on a received message has been replayed
    nonce_replay = 0x8,
    /// A nonce has reached a configured maximum value
    max_nonce_exceeded = 0x9,
    /// The session time has reached a configured maximum value
    max_session_time_exceeded = 0xA,
    /// Received a session message with no user data
    empty_user_data = 0xB,
    /// A rollback of the monotonic clock was detected
    clock_rollback = 0xC
};

struct CryptoErrorSpec : private openpal::StaticOnly
{
    typedef CryptoError enum_type_t;

    static const char* name;

    static const char* to_string(CryptoError arg);
};

typedef ErrorCategory<CryptoErrorSpec> CryptoErrorCategory;

inline std::error_code make_error_code(CryptoError err)
{
    return std::error_code(static_cast<int>(err), CryptoErrorCategory::get());
}

}

namespace std {

template <>
struct is_error_code_enum<ssp21::CryptoError> : public true_type {};

}

#endif

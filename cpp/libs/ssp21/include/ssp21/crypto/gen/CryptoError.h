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
#include "ser4cpp/util/Uncopyable.h"
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
    /// attempted operation with a buffer with the wrong length
    bad_length = 0x2,
    /// X25519 DH operation failed. Bad public key?
    dh_x25519_fail = 0x3,
    /// a procedure cannot be executed b/c of an undefined algorithm
    undefined_algorithm = 0x4,
    /// A calculated MAC did not match the specified MAC
    mac_auth_fail = 0x5,
    /// A calculated digital signature did not authenticate
    signature_auth_fail = 0x6,
    /// A buffer had an insufficient or incorrect size
    bad_buffer_size = 0x7,
    /// No validate session for authenticating user data
    no_valid_session = 0x8,
    /// The TTL on a message is expired
    expired_ttl = 0x9,
    /// The nonce on a received message has been replayed
    nonce_replay = 0xA,
    /// A nonce has reached a configured maximum value
    max_nonce_exceeded = 0xB,
    /// The session time has reached a configured maximum value
    max_session_time_exceeded = 0xC,
    /// Received a session message with no user data
    empty_user_data = 0xD,
    /// A rollback of the monotonic clock was detected
    clock_rollback = 0xE,
    /// AEAD encryption failed in the underlying implementation
    aead_encrypt_fail = 0xF,
    /// AEAD authentication failed in the underlying implementation
    aead_decrypt_fail = 0x10
};

struct CryptoErrorSpec : private ser4cpp::StaticOnly
{
    using enum_type_t = CryptoError;

    static const char* name;

    static const char* to_string(CryptoError arg);
};

using CryptoErrorCategory = ErrorCategory<CryptoErrorSpec>;

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

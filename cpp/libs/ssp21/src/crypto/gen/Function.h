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

#ifndef SSP21_FUNCTION_H
#define SSP21_FUNCTION_H

#include <cstdint>
#include "ser4cpp/util/Uncopyable.h"

namespace ssp21 {

/**
    Crypto layer function code
*/
enum class Function : uint8_t
{
    /// Initiator sends this to start a handshake
    request_handshake_begin = 0x0,
    /// Responder replies to the handshake begin from the initiator
    reply_handshake_begin = 0x1,
    /// Responder returns an error code to the master
    reply_handshake_error = 0x2,
    /// Both parties transmit session data messages
    session_data = 0x3,
    /// value not defined
    undefined = 0xFF
};

struct FunctionSpec : private ser4cpp::StaticOnly
{
    using enum_type_t = Function;

    static uint8_t to_type(Function arg);
    static Function from_type(uint8_t arg);
    static const char* to_string(Function arg);
};

}

#endif

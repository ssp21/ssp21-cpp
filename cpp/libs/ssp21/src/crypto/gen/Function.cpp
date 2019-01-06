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

#include "crypto/gen/Function.h"

namespace ssp21 {

uint8_t FunctionSpec::to_type(Function arg)
{
    return static_cast<uint8_t>(arg);
}
Function FunctionSpec::from_type(uint8_t arg)
{
    switch(arg)
    {
        case(0x0):
            return Function::request_handshake_begin;
        case(0x1):
            return Function::reply_handshake_begin;
        case(0x2):
            return Function::reply_handshake_error;
        case(0x3):
            return Function::session_data;
        default:
            return Function::undefined;
    }
}
const char* FunctionSpec::to_string(Function arg)
{
    switch(arg)
    {
        case(Function::request_handshake_begin):
            return "request_handshake_begin";
        case(Function::reply_handshake_begin):
            return "reply_handshake_begin";
        case(Function::reply_handshake_error):
            return "reply_handshake_error";
        case(Function::session_data):
            return "session_data";
        default:
            return "undefined";
    }
}

}

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

#include "ssp21/crypto/gen/RequestHandshakeBegin.h"

#include "ssp21/crypto/MessageParser.h"
#include "ssp21/crypto/MessagePrinting.h"
#include "ssp21/crypto/MessageFormatter.h"

namespace ssp21 {

RequestHandshakeBegin::RequestHandshakeBegin()
{}

RequestHandshakeBegin::RequestHandshakeBegin(
    uint16_t version,
    const CryptoSpec& spec,
    const SessionConstraints& constraints,
    HandshakeMode handshake_mode,
    const seq32_t& ephemeral_data,
    const seq32_t& handshake_data
) :
    version(version),
    spec(spec),
    constraints(constraints),
    handshake_mode(handshake_mode),
    ephemeral_data(ephemeral_data),
    handshake_data(handshake_data)
{}

size_t RequestHandshakeBegin::size() const
{
    return MessageFormatter::sum_sizes(
        1,
        version,
        spec,
        constraints,
        handshake_mode,
        ephemeral_data,
        handshake_data
    );
}


ParseError RequestHandshakeBegin::read(seq32_t input)
{
    auto read_fields = [this](seq32_t& input) -> ParseError 
    {
        return MessageParser::read_fields(
            input,
            version,
            spec,
            constraints,
            handshake_mode,
            ephemeral_data,
            handshake_data
        );
    };

    return MessageParser::read_message(input, Function::request_handshake_begin, read_fields);
}

FormatResult RequestHandshakeBegin::write(wseq32_t& output) const
{
    auto write_fields = [this](wseq32_t& output) -> FormatError 
    {
        return MessageFormatter::write_fields(
            output,
            version,
            spec,
            constraints,
            handshake_mode,
            ephemeral_data,
            handshake_data
        );
    };

    return MessageFormatter::write_message(output, Function::request_handshake_begin, write_fields);
}
void RequestHandshakeBegin::print(IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "version",
        version,
        "spec",
        spec,
        "constraints",
        constraints,
        "handshake_mode",
        handshake_mode,
        "ephemeral_data",
        ephemeral_data,
        "handshake_data",
        handshake_data
    );
}


}

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

#ifndef SSP21_REPLYHANDSHAKEAUTH_H
#define SSP21_REPLYHANDSHAKEAUTH_H

#include "openpal/util/Uncopyable.h"
#include "openpal/container/WSlice.h"
#include "openpal/container/RSlice.h"
#include "ssp21/gen/Function.h"
#include "ssp21/ILinePrinter.h"
#include "ssp21/FormatResult.h"
#include "ssp21/SequenceTypes.h"
#include "ssp21/gen/ParseError.h"
#include "ssp21/gen/FormatError.h"

namespace ssp21 {

struct ReplyHandshakeAuth : private openpal::Uncopyable
{
  ReplyHandshakeAuth();

  ReplyHandshakeAuth(
    const Seq8& mac
  );

  ParseError read_msg(const openpal::RSlice& input);
  FormatResult write_msg(openpal::WSlice& output) const;

  void print(ILinePrinter& printer) const;

  static const uint32_t min_size_bytes = 2;

  static const Function function = Function::request_handshake_auth;

  Seq8 mac;

  private: 

  FormatError write(openpal::WSlice& output) const;
};

}

#endif

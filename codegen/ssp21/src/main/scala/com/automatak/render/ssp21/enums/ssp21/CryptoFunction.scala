/**
 * License TBD
 */
package com.automatak.render.ssp21.enums.ssp21

import com.automatak.render._

object CryptoFunction {

  private val comments = List("Crypto layer function code")

  def apply(): EnumModel = EnumModel(
    "Function",
    "function",
    comments,
    EnumModel.UInt8,
    codes,
    Some(EnumValue.undefined(255)),
    None,
    Hex
  )

  val requestHandshakeBegin = EnumValue("request_handshake_begin", 0, "Master sends this initiate a handshake")
  val replyHandshakeBegin = EnumValue("reply_handshake_begin", 1, "Outstation replies to the handshake begin from the master")
  val requestHandshakeAuth = EnumValue("request_handshake_auth", 2, "Master authenticates to the outstation")
  val replyHandshakeAuth = EnumValue("reply_handshake_auth", 3, "Outstation authenticates to the master")
  val replyHandshakeError = EnumValue("reply_handshake_error", 4, "Outstation returns an error code to the master")
  val unconfirmedSessionData = EnumValue("unconfirmed_session_data", 5, "Either party transmits an unconfirmed session message")

  private val codes = List(
    requestHandshakeBegin,
    replyHandshakeBegin,
    requestHandshakeAuth,
    replyHandshakeAuth,
    replyHandshakeError,
    unconfirmedSessionData
  )

}




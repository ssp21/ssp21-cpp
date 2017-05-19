/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.ssp21

import com.automatak.render._

object CryptoFunction extends EnumModel {

  override def name: String = "Function"

  override def underscoredName: String = "function"

  override def comments: List[String] = List("Crypto layer function code")

  override def nonDefaultValues: List[EnumValue] = codes

  override def defaultValue: Option[EnumValue] = Some(EnumValue.undefined(255))

  val requestHandshakeBegin = EnumValue("request_handshake_begin", 0, "Master sends this initiate a handshake")
  val replyHandshakeBegin = EnumValue("reply_handshake_begin", 1, "Outstation replies to the handshake begin from the master")
  val requestHandshakeAuth = EnumValue("request_handshake_auth", 2, "Master authenticates to the outstation")
  val replyHandshakeAuth = EnumValue("reply_handshake_auth", 3, "Outstation authenticates to the master")
  val replyHandshakeError = EnumValue("reply_handshake_error", 4, "Outstation returns an error code to the master")
  val sessionData = EnumValue("session_data", 5, "Both parties transmit session data messages")

  private val codes = List(
    requestHandshakeBegin,
    replyHandshakeBegin,
    requestHandshakeAuth,
    replyHandshakeAuth,
    replyHandshakeError,
    sessionData
  )

}




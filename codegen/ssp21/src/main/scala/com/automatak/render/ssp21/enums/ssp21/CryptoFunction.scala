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

  val requestHandshakeBegin = EnumValue("request_handshake_begin", 0, "Initiator sends this to start a handshake")
  val replyHandshakeBegin = EnumValue("reply_handshake_begin", 1, "Responder replies to the handshake begin from the initiator")
  val replyHandshakeError = EnumValue("reply_handshake_error", 2, "Responder returns an error code to the master")
  val sessionData = EnumValue("session_data", 3, "Both parties transmit session data messages")

  private val codes = List(
    requestHandshakeBegin,
    replyHandshakeBegin,
    replyHandshakeError,
    sessionData
  )

}




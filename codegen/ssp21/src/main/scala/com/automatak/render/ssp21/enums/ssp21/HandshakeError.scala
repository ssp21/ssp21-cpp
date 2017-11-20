/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.ssp21

import com.automatak.render._

object HandshakeError extends EnumModel {

  override def name: String = "HandshakeError"

  override def underscoredName: String = "handshake_error"

  override def comments: List[String] = List("Denotes an error condition that occurred during the handshake process")

  override def nonDefaultValues: List[EnumValue] = codes

  override def defaultValue: Option[EnumValue] = Some(EnumValue.undefined(254))

  override def boolCastValue: Option[EnumValue] = Some(noneValue)

  private val noneValue = EnumValue("none", 253, "This value gets used internally in ssp21-cpp only")

  private val codes = List(
    EnumValue("bad_message_format", 0, "A received handshake message was malformed in some manner"),
    EnumValue("unsupported_version", 1, "The requested version is not supported"),
    EnumValue("unsupported_handshake_ephemeral", 2, "The requested handshake ephemeral is not supported or doesn't match the handshake mode"),
    EnumValue("unsupported_handshake_hash", 3, "The requested handshake hash is not supported"),
    EnumValue("unsupported_handshake_kdf", 4, "The requested handshake KDF is not supported"),
    EnumValue("unsupported_session_mode", 6, "The requested session mode is not supported"),
    EnumValue("unsupported_nonce_mode", 7, "The requested nonce verification mode is not supported"),
    EnumValue("unsupported_handshake_mode", 8, "The requested handshake mode is not supported"),
    EnumValue("bad_certificate_format", 9, "One of the received certificates was improperly encoded"),
    EnumValue("bad_certificate_chain", 10, "One of the received certificates was improperly encoded"),
    EnumValue("unsupported_certificate_feature", 11, "One of the received certificates utilizes an unsupported feature"),
    EnumValue("authentication_error", 12, "The outstation was unable to authenticate the master"),
    EnumValue("no_prior_handshake_begin", 13, "The master requested handshake auth, but no prior handshake begin was received"),
    EnumValue("key_not_found", 14, "In shared-secret mode, the requested key id (mode_data) was not found"),
    EnumValue("internal", 255, "internal error"),
    noneValue
  )

}




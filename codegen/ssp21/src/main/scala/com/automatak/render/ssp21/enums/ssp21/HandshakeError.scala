/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.ssp21

import com.automatak.render._

object HandshakeError {

  private val comments = List("Denotes an error condition that occurred during the handshake process")

  def apply(): EnumModel = EnumModel("HandshakeError", "handshake_error", comments, EnumType.UInt8, codes, Some(EnumValue.undefined(254)), Some(noneValue), Hex)

  private val noneValue = EnumValue("none", 253, "This value gets used internally in ssp21-cpp only")

  private val codes = List(
    EnumValue("bad_message_format", 0, "A received handshake message was malformed in some manner"),
    EnumValue("unsupported_version", 1, "The requested version is not supported"),
    EnumValue("unsupported_dh_mode", 2, "The requested DH mode is not supported"),
    EnumValue("unsupported_handshake_hash", 3, "The requested handshake hash is not supported"),
    EnumValue("unsupported_handshake_kdf", 4, "The requested handshake KDF is not supported"),
    EnumValue("unsupported_handshake_mac", 5, "The requested handshake MAC is not supported"),
    EnumValue("unsupported_session_mode", 6, "The requested session mode is not supported"),
    EnumValue("unsupported_nonce_mode", 7, "The requested nonce verification mode is not supported"),
    EnumValue("unsupported_certificate_mode", 8, "The requested certificate mode is not supported"),
    EnumValue("bad_certificate_format", 9, "One of the received certificates was improperly encoded"),
    EnumValue("unsupported_certificate_feature", 10, "One of the received certificates utilizes an unsupported feature"),
    EnumValue("authentication_error", 11, "The outstation was unable to authenticate the master"),
    EnumValue("no_prior_handshake_begin", 12, "The master requested handshake auth, but no prior handshake begin was received"),
    EnumValue("internal", 255, "internal error"),
    noneValue
  )

}




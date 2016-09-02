/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.ssp21

import com.automatak.render._

object HandshakeError {

  private val comments = List("Denotes an error condition that occurred during the handshake process")

  def apply(): EnumModel = EnumModel("HandshakeError", "handshake_error", comments, EnumModel.UInt8, codes, Some(EnumValue.undefined(255)), Some(falseValue), Hex)

  private val falseValue = EnumValue("none", 254, "This value gets used internally in ssp21-cpp only")

  private val codes = List(
    EnumValue("bad_message_format", 0, "A received handshake message was malformed in some manner"),
    EnumValue("unsupported_version", 1, "The requested version is not supported"),
    EnumValue("unsupported_dh_mode", 2, "The requested DH mode is not supported"),
    EnumValue("unsupported_hash_mode", 3, "The requested hash mode is not supported"),
    EnumValue("unsupported_session_mode", 4, "The requested session mode is not supported"),
    EnumValue("unsupported_nonce_mode", 5, "The requested nonce verification mode is not supported"),
    EnumValue("unsupported_certificate_mode", 6, "The requested certificate mode is not supported"),
    EnumValue("bad_certificate_format", 7, "One of the received certificates was improperly encoded"),
    EnumValue("unsupported_certificate_feature", 8, "One of the received certificates utilizes an unsupported feature"),
    EnumValue("authentication_error", 9, "The outstation was unable to authenticate the master"),
    EnumValue("no_prior_handshake_begin", 10, "The master requested handshake auth, but no prior handshake begin was received"),
    EnumValue("internal", 11, "internal error"),

    falseValue
  )

}




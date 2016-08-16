/**
 * License TBD
 */
package com.automatak.render.ssp21.enums.ssp21

import com.automatak.render._

object HandshakeError {

  private val comments = List("Denotes an error condition that occurred during the handshake process")

  def apply(): EnumModel = EnumModel("HandshakeError", "handshake_error", comments, EnumModel.UInt8, codes, Some(defaultValue), None, Hex)

  private val defaultValue = EnumValue("undefined", 254, "undefined error")

  private val codes = List(
    EnumValue("bad_message_format", 0, "A received handshake message was malformed in some manner"),
    EnumValue("unsupported_dh_mode", 1, "The requested DH mode is not supported"),
    EnumValue("unsupported_hash_mode", 2, "The requested hash mode is not supported"),
    EnumValue("unsupported_session_mode", 3, "The requested session mode is not supported"),
    EnumValue("unsupported_certificate_mode", 4, "The requested certificate mode is not supported"),
    EnumValue("bad_certificate_format", 5, "One of the received certificates was improperly encoded"),
    EnumValue("unsupported_certificate_feature", 6, "One of the received certificates utilizes an unsupported feature"),
    EnumValue("authentication_error", 7, "The outstation was unable to authenticate the master"),
    EnumValue("internal", 255, "internal error")
  )

}




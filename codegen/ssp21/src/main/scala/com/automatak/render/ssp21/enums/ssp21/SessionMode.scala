/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.ssp21

import com.automatak.render._

object SessionMode {

  private val comments = List("Specifies the complete set of algorithms used to secure the session")

  def apply(): EnumModel = EnumModel("SessionMode", "session_mode", comments, EnumModel.UInt8, codes, Some(EnumValue.undefined(255)), None, Hex)

  private val codes = List(
    EnumValue("hmac_sha256_16", 0, "HMAC-SHA256 truncated to 16 bytes")
  )

}




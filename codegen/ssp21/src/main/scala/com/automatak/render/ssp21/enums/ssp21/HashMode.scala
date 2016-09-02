/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.ssp21

import com.automatak.render._

object HashMode {

  private val comments = List("Specifies which hash algorithm is used for the handshake hash and handshake authentication")

  def apply(): EnumModel = EnumModel("HashMode", "hash_mode", comments, EnumModel.UInt8, codes, Some(EnumValue.undefined(255)), None, Hex)

  private val codes = List(
    EnumValue("sha256", 0, "Use SHA256 and HMAC-SHA256")
  )

}




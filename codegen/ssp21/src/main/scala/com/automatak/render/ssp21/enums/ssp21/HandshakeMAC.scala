/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.ssp21

import com.automatak.render._

object HandshakeMAC {

  private val comments = List("Specifies the Message Authentication Code (MAC) algorithm used to authenticate the handshake")

  def apply(): EnumModel = EnumModel("HandshakeMAC", "handshake_mac", comments, EnumType.UInt8, codes, Some(EnumValue.undefined(255)), None, Hex)

  private val codes = List(
    EnumValue("HMAC_SHA256", 0, "Use HMAC-SHA256")
  )

}




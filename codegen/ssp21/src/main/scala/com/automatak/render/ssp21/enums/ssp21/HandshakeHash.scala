/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.ssp21

import com.automatak.render._

object HandshakeHash {

  private val comments = List("Specifies which hash algorithm is used for the handshake hash to calculate the chaining key")

  def apply(): EnumModel = EnumModel("HandshakeHash", "handshake_hash", comments, EnumType.UInt8, codes, Some(EnumValue.undefined(255)), None, Hex)

  private val codes = List(
    EnumValue("sha256", 0, "Use SHA256")
  )

}




/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.ssp21

import com.automatak.render._

object HandshakeKDF {

  private val comments = List("Specifies the Key Derivation Function (KDF) used during the handshake")

  def apply(): EnumModel = EnumModel("HandshakeKDF", "handshake_kdf", comments, EnumType.UInt8, codes, Some(EnumValue.undefined(255)), None, Hex)

  private val codes = List(
    EnumValue("HKDF_SHA256", 0, "Use HKDF with HMAC-SHA256")
  )

}




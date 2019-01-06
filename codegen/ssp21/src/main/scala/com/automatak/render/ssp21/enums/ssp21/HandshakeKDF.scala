/**
 * License TBD
 */
/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.ssp21

import com.automatak.render._

object HandshakeKDF extends EnumModel {

  override def name: String = "HandshakeKDF"

  override def underscoredName: String = "handshake_kdf"

  override def comments: List[String] = List("Specifies the Key Derivation Function (KDF) used during the handshake")

  override def nonDefaultValues: List[EnumValue] = codes

  override def defaultValue: Option[EnumValue] = Some(EnumValue.undefined(255))

  private val codes = List(
    EnumValue("hkdf_sha256", 0, "Use HKDF with HMAC-SHA256")
  )

}




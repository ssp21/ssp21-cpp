/**
 * License TBD
 */
/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.ssp21

import com.automatak.render._

object HandshakeMode extends EnumModel {

  override def name: String = "HandshakeMode"

  override def underscoredName: String = "handshake_mode"

  override def comments: List[String] = List("specifies how key derivation occurs")

  override def nonDefaultValues: List[EnumValue] = codes

  override def defaultValue: Option[EnumValue] = Some(EnumValue.undefined(255))

  private val codes = List(
    EnumValue("shared_secret", 0, "both parties have a shared secret"),
    EnumValue("public_keys", 1, "both parties know each others public DH keys"),
    EnumValue("quantum_key_distribution", 2, "Single-use shared secrets delivered via QKD"),
    EnumValue("industrial_certificates", 3, "Industrial certificate format"),
  )

}




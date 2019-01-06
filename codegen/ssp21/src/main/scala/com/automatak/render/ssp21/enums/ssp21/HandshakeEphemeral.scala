/**
 * License TBD
 */
/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.ssp21

import com.automatak.render._

object HandshakeEphemeral extends EnumModel {

  override def name: String = "HandshakeEphemeral"

  override def underscoredName: String = "handshake_ephemeral"

  override def comments: List[String] = List("Specifies the meaning of the ephemeral data in RequestHandshakeBegin")

  override def nonDefaultValues: List[EnumValue] = codes

  override def defaultValue: Option[EnumValue] = Some(EnumValue.undefined(255))

  private val codes = List(
    EnumValue("x25519", 0, "x25519 algorithm public key"),
    EnumValue("nonce", 1, "32-byte random nonce"),
    EnumValue("none", 2, "Empty ephemeral used for QKD mode")
  )

}




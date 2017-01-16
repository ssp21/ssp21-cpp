/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.ssp21

import com.automatak.render._

object HandshakeMAC extends EnumModel {

  override def name: String = "HandshakeMAC"

  override def underscoredName: String = "handshake_mac"

  override def comments: List[String] = List("Specifies the Message Authentication Code (MAC) algorithm used to authenticate the handshake")

  override def nonDefaultValues: List[EnumValue] = codes

  override def defaultValue: Option[EnumValue] = Some(EnumValue.undefined(255))

  private val codes = List(
    EnumValue("hmac_sha256", 0, "Use HMAC-SHA256")
  )

}




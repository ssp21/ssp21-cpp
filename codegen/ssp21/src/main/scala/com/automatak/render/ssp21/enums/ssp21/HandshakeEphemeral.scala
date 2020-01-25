package com.automatak.render.ssp21.enums.ssp21

import com.automatak.render.{EnumModel, EnumValue}


object HandshakeEphemeral extends EnumModel {

  override def name: String = "HandshakeEphemeral"

  override def underscoredName: String = "handshake_ephemeral"

  override def comments: List[String] = List("Type of ephemeral value used during the handshake")

  override def nonDefaultValues: List[EnumValue] = codes

  override def defaultValue: Option[EnumValue] = Some(EnumValue.undefined(255))

  private val codes = List(
    EnumValue("x25519", 0, "x25519"),
    EnumValue("nonce", 1, "nonce"),
    EnumValue("none", 2, "none"),
  )

}


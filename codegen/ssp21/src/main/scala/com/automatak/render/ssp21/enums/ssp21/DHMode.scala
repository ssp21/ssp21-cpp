/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.ssp21

import com.automatak.render._

object DHMode extends EnumModel {

  override def name: String = "DHMode"

  override def underscoredName: String = "dh_mode"

  override def comments: List[String] = List("Specifies which Diffie Hellman function is used during the handshake")

  override def nonDefaultValues: List[EnumValue] = codes

  override def defaultValue: Option[EnumValue] = Some(EnumValue.undefined(255))

  private val codes = List(
    EnumValue("x25519", 0, "Use the x25519 algorithm")
  )

}




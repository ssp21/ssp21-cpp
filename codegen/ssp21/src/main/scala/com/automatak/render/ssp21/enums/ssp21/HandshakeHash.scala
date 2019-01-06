/**
 * License TBD
 */
/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.ssp21

import com.automatak.render._

object HandshakeHash extends EnumModel {

  override def name: String = "HandshakeHash"

  override def underscoredName: String = "handshake_hash"

  override def comments: List[String] = List("Specifies which hash algorithm is used for the handshake hash to calculate the chaining key")

  override def nonDefaultValues: List[EnumValue] = codes

  override def defaultValue: Option[EnumValue] = Some(EnumValue.undefined(255))

  private val codes = List(
    EnumValue("sha256", 0, "Use SHA256")
  )

}




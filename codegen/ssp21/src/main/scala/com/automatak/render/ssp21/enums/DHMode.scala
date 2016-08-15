/**
 * License TBD
 */
package com.automatak.render.ssp21.enums

import com.automatak.render._

object DHMode {

  private val comments = List("Specifies which Diffie Hellman function is used")

  def apply(): EnumModel = EnumModel("DHMode", comments, EnumModel.UInt8, codes, Some(defaultValue), Hex)

  private val defaultValue = EnumValue("undefined", 255, "undefined mode")

  private val codes = List(
    EnumValue("x25519", 0, "Use the x25519 algorithm")
  )

}




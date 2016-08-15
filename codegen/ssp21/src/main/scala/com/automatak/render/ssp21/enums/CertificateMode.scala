/**
 * License TBD
 */
package com.automatak.render.ssp21.enums

import com.automatak.render._

object CertificateMode {

  private val comments = List("specifies what type of certificate is exchanged by both parties to authenticate each other")

  def apply(): EnumModel = EnumModel("CertificateMode", "certificate_mode", comments, EnumModel.UInt8, codes, Some(defaultValue), Hex)

  private val defaultValue = EnumValue("undefined", 255, "undefined mode")

  private val codes = List(
    EnumValue("preshared_keys", 0, "both parties know each others public DH keys"),
    EnumValue("m2m", 1, "machine-to-machine certificate format")
  )

}




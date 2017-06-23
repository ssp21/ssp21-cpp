/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.ssp21

import com.automatak.render._

object CertificateMode extends EnumModel {

  override def name: String = "CertificateMode"

  override def underscoredName: String = "certificate_mode"

  override def comments: List[String] = List("specifies what type of certificate is exchanged by both parties to authenticate each other")

  override def nonDefaultValues: List[EnumValue] = codes

  override def defaultValue: Option[EnumValue] = Some(EnumValue.undefined(255))

  private val codes = List(
    EnumValue("preshared_keys", 0, "both parties know each others public DH keys"),
    EnumValue("icf", 1, "Industrial certificate format")
  )

}




/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.internal

import com.automatak.render._

object PEMDecodeError extends EnumModel {

  override def name: String = "PEMDecodeError"

  override def underscoredName: String = "pem_decode_error"

  override def comments: List[String] = List("The result of a PEM decode operation")

  override def nonDefaultValues: List[EnumValue] = codes

  override def defaultValue: Option[EnumValue] = None

  override def boolCastValue: Option[EnumValue] = Some(falseValue)

  override def errorCategory: Option[ErrorCategory] = Some(ErrorCategory("PEMDecodeErrorCategory", "PEM decode error"))

  private val falseValue = EnumValue("ok", 0, "decode success")

  private val codes = List(
    falseValue,
    EnumValue("end_of_file", 1, "End of file was reached before a complete decoding"),
    EnumValue("missing_begin_header", 2, "a -----BEGIN <id>----- header was missing")
  )

}





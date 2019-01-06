/**
 * License TBD
 */
/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.internal

import com.automatak.render._

object Base64DecodeError extends EnumModel {

  override def name: String = "Base64DecodeError"

  override def underscoredName: String = "base64_decode_error"

  override def comments: List[String] = List("The result of a base64 decode operation")

  override def nonDefaultValues: List[EnumValue] = codes

  override def defaultValue: Option[EnumValue] = None

  override def boolCastValue: Option[EnumValue] = Some(falseValue)

  private val falseValue = EnumValue("ok", 0, "decode success")

  private val codes = List(
    falseValue,
    EnumValue("not_mult_four", 1, "The base64 input is not a multiple of four"),
    EnumValue("not_base64", 2, "The input contains a non-base64 value"),
    EnumValue("bad_trailing_input", 3, "Trailing input after terminating characters")
  )

}



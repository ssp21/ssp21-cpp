/**
 * License TBD
 */
/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.internal

import com.automatak.render._

object FormatError extends EnumModel {

  override def name: String = "FormatError"

  override def underscoredName: String = "format_error"

  override def comments: List[String] = List("The result of a message format operation")

  override def nonDefaultValues: List[EnumValue] = codes

  override def defaultValue: Option[EnumValue] = None

  override def errorCategory = Some(ErrorCategory("FormatErrorCategory", "message format error"))

  override def boolCastValue: Option[EnumValue] = Some(falseValue)

  private val falseValue = EnumValue("ok", 0, "message was formatted successfully")

  private val codes = List(
    falseValue,
    EnumValue("insufficient_space", 1, "not enough output buffer space")
  )

}




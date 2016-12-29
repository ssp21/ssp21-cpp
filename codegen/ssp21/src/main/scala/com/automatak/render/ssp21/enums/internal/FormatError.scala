/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.internal

import com.automatak.render._

object FormatError {

  private val comments = List("The result of a message format operation")

  def category = Some(ErrorCategory("FormatErrorCategory", "message format error"))

  def apply(): EnumModel = EnumModel("FormatError", "format_error", comments, EnumType.UInt8, codes, None, Some(falseValue), Hex, category)

  private val falseValue = EnumValue("ok", 0, "message was formatted successfully")

  private val codes = List(
    falseValue,
    EnumValue("insufficient_space", 1, "not enough output buffer space")
  )

}




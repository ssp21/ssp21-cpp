/**
 * License TBD
 */
package com.automatak.render.ssp21.enums.internal

import com.automatak.render._

object ParseError {

  private val comments = List("The result of a message parse operation")

  def apply(): EnumModel = EnumModel("ParseError", "parse_error", comments, EnumModel.UInt8, codes, None, Hex)

  private val codes = List(
    EnumValue("ok", 0, "message was parsed successfully"),
    EnumValue("insufficient_bytes", 1, "parser ran out of bytes before completion"),
    EnumValue("undefined_enum", 2, "an enumeration value was undefined"),
    EnumValue("unexpected_function", 3, "the message function was not the expected value"),
    EnumValue("too_many_bytes", 4, "parser completed with bytes still remaining"),
    EnumValue("impl_capacity_limit", 5, "reached an implementation specific capacity limit")
  )

}




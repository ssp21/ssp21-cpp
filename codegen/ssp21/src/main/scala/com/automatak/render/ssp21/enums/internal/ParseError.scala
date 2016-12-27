/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.internal

import com.automatak.render._

object ParseError {

  private val comments = List("The result of a message parse operation")

  def apply(): EnumModel = EnumModel("ParseError", "parse_error", comments, EnumType.UInt8, codes, None, Some(falseValue), Hex)

  private val falseValue = EnumValue("ok", 0, "message was parsed successfully")

  private val codes = List(
    falseValue,
    EnumValue("insufficient_bytes", 1, "parser ran out of bytes before completion"),
    EnumValue("undefined_enum", 2, "an enumeration value was undefined"),
    EnumValue("unexpected_function", 3, "the message function was not the expected value"),
    EnumValue("too_many_bytes", 4, "parser completed with bytes still remaining"),
    EnumValue("impl_capacity_limit", 5, "reached an implementation specific capacity limit"),
    EnumValue("reserved_bit", 6, "the bitfield contains a reserved bit(s) that is set")
  )

}




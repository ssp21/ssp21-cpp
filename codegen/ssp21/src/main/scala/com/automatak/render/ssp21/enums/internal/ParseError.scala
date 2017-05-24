/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.internal

import com.automatak.render._

object ParseError extends EnumModel {

  override def name: String = "ParseError"

  override def underscoredName: String = "parse_error"

  override def comments: List[String] = List("The result of a message parse operation")

  override def nonDefaultValues: List[EnumValue] = codes

  override def defaultValue: Option[EnumValue] = None

  override def boolCastValue: Option[EnumValue] = Some(falseValue)

  private val falseValue = EnumValue("ok", 0, "message was parsed successfully")

  private val codes = List(
    falseValue,
    EnumValue("insufficient_bytes", 1, "parser ran out of bytes before completion"),
    EnumValue("undefined_enum", 2, "an enumeration value was undefined"),
    EnumValue("unexpected_function", 3, "the message function was not the expected value"),
    EnumValue("too_many_bytes", 4, "parser completed with bytes still remaining"),
    EnumValue("impl_capacity_limit", 5, "reached an implementation specific capacity limit"),
    EnumValue("reserved_bit", 6, "the bit-field contains a reserved bit(s) that is set"),
    EnumValue("bad_vlength", 7, "a variable length (vlength) was not encoded correctly")
  )

}




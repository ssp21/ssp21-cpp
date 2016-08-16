/**
 * License TBD
 */
package com.automatak.render.ssp21.enums.internal

import com.automatak.render._

object ParseResult {

  private val comments = List("The result of a message parse operation")

  def apply(): EnumModel = EnumModel("ParseResult", "parse_result", comments, EnumModel.UInt8, codes, None, Hex)

  private val codes = List(
    EnumValue("ok", 0, "message was parsed successfully"),
    EnumValue("insufficient_bytes", 1, "parser ran out of bytes before completion"),
    EnumValue("too_many_bytes", 2, "parser completed with bytes still remaining")
  )

}




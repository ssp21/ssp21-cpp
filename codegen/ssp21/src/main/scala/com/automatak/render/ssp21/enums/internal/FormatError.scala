/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.internal

import com.automatak.render._

object FormatError {

  private val comments = List("The result of a message format operation")

  def apply(): EnumModel = EnumModel("FormatError", "format_error", comments, EnumModel.UInt8, codes, None, Some(falseValue), Hex)

  private val falseValue = EnumValue("ok", 0, "message was formatted successfully")

  private val codes = List(
    falseValue,
    EnumValue("insufficient_space", 1, "not enough output buffer space"),
    EnumValue("bad_sequence_length", 2, "a message sequence length overflowed its serialized representation")
  )

}




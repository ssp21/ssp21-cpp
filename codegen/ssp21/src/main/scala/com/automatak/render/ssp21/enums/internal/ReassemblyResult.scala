/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.internal

import com.automatak.render._

object ReassemblyResult {

  private val comments = List("The result of a message reassembly operation")

  def apply(): EnumModel = EnumModel("ReassemblyResult", "reassembly_result", comments, EnumModel.UInt8, codes, None, None, Hex, None)

  private val codes = List(
    EnumValue("complete", 0, "reassembly complete"),
    EnumValue("partial", 1, "partial reassembly. more data needed"),
    EnumValue("bad_nonce", 2, "reassembly failed due to an out-of-order nonce")
  )

}




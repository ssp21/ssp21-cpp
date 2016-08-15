/**
 * License TBD
 */
package com.automatak.render.ssp21.enums

import com.automatak.render._

object NonceMode {

  private val comments = List("Determines how nonces are verified during the session")

  def apply(): EnumModel = EnumModel("NonceMode", comments, EnumModel.UInt8, codes, Some(defaultValue), Hex)

  private val defaultValue = EnumValue("undefined", 255, "undefined mode")

  private val codes = List(
    EnumValue("increment_last_rx", 0, "new nonce must strictly be equal to last nonce plus one"),
    EnumValue("greater_than_last_rx", 1, "new nonce must be greater than last nonce")
  )

}




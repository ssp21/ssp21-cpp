/**
 * License TBD
 */
package com.automatak.render.ssp21.enums

import com.automatak.render._

object CryptoFunction {

  private val comments = List("Crypto layer function code")

  def apply(): EnumModel = EnumModel("Function", "function", comments, EnumModel.UInt8, codes, Some(defaultValue), Hex)

  private val defaultValue = EnumValue("undefined", 255, "Unknown function code. Used internally in ssp21 to indicate the code didn't match anything known")

  private val codes = List(
    EnumValue("request_handshake_begin", 0, "Master sends this initiate a handshake"),
    EnumValue("reply_handshake_begin", 1, "Outstation replies to the handshake begin from the master"),
    EnumValue("request_handshake_auth", 2, "Master authenticates to the outstation"),
    EnumValue("reply_handshake_auth", 3, "Outstation authenticates to the master"),
    EnumValue("reply_handshake_error", 4, "Outstation returns an error code to the master"),
    EnumValue("unconfirmed_session_data", 5, "Either party transmits an unconfirmed session message")
  )

}




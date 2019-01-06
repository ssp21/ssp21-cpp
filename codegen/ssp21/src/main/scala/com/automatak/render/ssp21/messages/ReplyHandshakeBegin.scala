/**
 * License TBD
 */
package com.automatak.render.ssp21.messages

import com.automatak.render.ssp21.enums.ssp21.CryptoFunction

object ReplyHandshakeBegin extends Message {

  override def name: String = "ReplyHandshakeBegin"

  def function = CryptoFunction.replyHandshakeBegin

  override def fields: List[Field] = List(
    CommonFields.ephemeralData,
    CommonFields.handshakeData
  )

}

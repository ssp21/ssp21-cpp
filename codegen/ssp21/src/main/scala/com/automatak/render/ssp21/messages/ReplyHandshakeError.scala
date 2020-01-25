/**
 * License TBD
 */
package com.automatak.render.ssp21.messages

import com.automatak.render.EnumValue
import com.automatak.render.ssp21.enums.ssp21.{CryptoFunction, HandshakeError}

object ReplyHandshakeError extends Message {

  override def name: String = "ReplyHandshakeError"

  def function: EnumValue = CryptoFunction.replyHandshakeError

  override def fields: List[Field] = List(
    Enum(HandshakeError)
  )

}

package com.automatak.render.ssp21.messages

import com.automatak.render.ssp21.enums._

object ReplyHandshakeError extends Message {

  override def name : String = "ReplyHandshakeError"

  override def fields: List[Field] = List(
      Enum(CryptoFunction()),
      Enum(HandshakeError())
  )

}

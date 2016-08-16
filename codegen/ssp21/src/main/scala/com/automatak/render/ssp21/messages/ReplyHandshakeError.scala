package com.automatak.render.ssp21.messages

import com.automatak.render.ssp21.enums._
import com.automatak.render.ssp21.enums.ssp21.{CryptoFunction, HandshakeError}

object ReplyHandshakeError extends Message {

  override def name : String = "ReplyHandshakeError"

  override def fields: List[Field] = List(
      FixedEnum(CryptoFunction(), CryptoFunction.replyHandshakeError),
      Enum(HandshakeError())
  )

}

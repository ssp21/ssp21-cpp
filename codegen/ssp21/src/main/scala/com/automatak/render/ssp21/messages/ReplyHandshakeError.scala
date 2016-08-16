package com.automatak.render.ssp21.messages

import com.automatak.render.EnumValue
import com.automatak.render.ssp21.enums.ssp21.{CryptoFunction, HandshakeError}

object ReplyHandshakeError extends Message {

  override def name : String = "ReplyHandshakeError"

  def enumValue : EnumValue = CryptoFunction.replyHandshakeError

  override def fields: List[Field] = List(
      FixedEnum(CryptoFunction(), CryptoFunction.replyHandshakeError),
      Enum(HandshakeError())
  )

}

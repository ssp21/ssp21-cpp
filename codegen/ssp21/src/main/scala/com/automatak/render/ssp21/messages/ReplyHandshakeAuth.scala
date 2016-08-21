package com.automatak.render.ssp21.messages

import com.automatak.render.ssp21.enums.ssp21.CryptoFunction


object ReplyHandshakeAuth extends Message {

  override def name : String = "ReplyHandshakeAuth"

  def function = CryptoFunction.requestHandshakeAuth

  override def fields: List[Field] = RequestHandshakeAuth.fields

}

package com.automatak.render.ssp21.messages

import com.automatak.render.ssp21.enums.ssp21.CryptoFunction

object RequestHandshakeAuth extends Message {

  override def name: String = "RequestHandshakeAuth"

  override def function = CryptoFunction.requestHandshakeAuth

  override def fields: List[Field] = List(Seq8("mac"))

}

package com.automatak.render.ssp21.messages


object ReplyHandshakeAuth extends Message {

  override def name : String = "ReplyHandshakeAuth"

  override def fields: List[Field] = RequestHandshakeAuth.fields

}

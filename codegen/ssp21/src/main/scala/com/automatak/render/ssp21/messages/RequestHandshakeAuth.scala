package com.automatak.render.ssp21.messages

import com.automatak.render.ssp21.enums._

object RequestHandshakeAuth extends Message {

  override def name : String = "RequestHandshakeAuth"

  override def fields: List[Field] = List(
      Enum(CryptoFunction()),
      Seq8("mac")
  )

}

package com.automatak.render.ssp21.messages

import com.automatak.render.ssp21.enums._

object RequestHandshakeAuth extends Message {

  override def name : String = "RequestHandshakeAuth"

  override def fields: List[Field] = List(
      FixedEnum(CryptoFunction(), CryptoFunction.requestHandshakeAuth),
      Seq8("mac")
  )

}

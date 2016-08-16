package com.automatak.render.ssp21.messages

import com.automatak.render.EnumValue
import com.automatak.render.ssp21.enums.ssp21.CryptoFunction

object RequestHandshakeAuth extends Message {

  override def name : String = "RequestHandshakeAuth"

  def enumValue : EnumValue = CryptoFunction.requestHandshakeAuth

  override def fields: List[Field] = List(
      FixedEnum(CryptoFunction(), CryptoFunction.requestHandshakeAuth),
      Seq8("mac")
  )

}

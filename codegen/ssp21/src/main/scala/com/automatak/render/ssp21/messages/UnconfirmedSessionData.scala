package com.automatak.render.ssp21.messages

import com.automatak.render.ssp21.enums._
import com.automatak.render.ssp21.enums.ssp21.CryptoFunction


object UnconfirmedSessionData extends Message {

  override def name : String = "UnconfirmedSessionData"

  override def fields: List[Field] = List(
      FixedEnum(CryptoFunction(), CryptoFunction.unconfirmedSessionData),
      U32("valid_until_ms"),
      U16("nonce"),
      Seq16("payload")
  )

}

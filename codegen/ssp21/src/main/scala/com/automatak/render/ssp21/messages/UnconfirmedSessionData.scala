package com.automatak.render.ssp21.messages

import com.automatak.render.ssp21.enums.ssp21.CryptoFunction


object UnconfirmedSessionData extends Message {

  override def name : String = "UnconfirmedSessionData"

  def function = CryptoFunction.unconfirmedSessionData

  override def fields: List[Field] = List(
      //Bitfield("flags", "Flags", List(Bit("fir", true), Bit("fin", true))),
      U32("valid_until_ms"),
      U16("nonce"),
      Seq16("payload")
  )

}

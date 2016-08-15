package com.automatak.render.ssp21.messages

import com.automatak.render.ssp21.enums._


object UnconfirmedSessionData extends Message {

  override def name : String = "UnconfirmedSessionData"

  override def fields: List[Field] = List(
      Enum(CryptoFunction()),
      U32("valid_until_ms"),
      U16("nonce"),
      Seq16ofU8("payload")
  )

}

package com.automatak.render.ssp21.messages

import com.automatak.render.ssp21.Bitfields

object AuthMetadata extends Struct {

  override def name: String = "AuthMetadata"

  override def fields: List[Field] = List(
    U32("valid_until_ms"),
    U16("nonce"),
    Bitfields.payloadFlags
  )

}

package com.automatak.render.ssp21.messages

import com.automatak.render.ssp21.Bitfields.SessionFlags

object AuthMetadata extends Struct {

  override def name: String = "AuthMetadata"

  override def fields: List[Field] = List(
    U16("nonce"),
    U32("valid_until_ms"),
    BitfieldField("flags", SessionFlags)
  )

}

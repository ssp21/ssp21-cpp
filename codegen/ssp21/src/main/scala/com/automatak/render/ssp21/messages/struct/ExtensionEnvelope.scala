package com.automatak.render.ssp21.messages.struct

import com.automatak.render.ssp21.messages._

object ExtensionEnvelope extends Struct {

  override def name: String = "ExtensionEnvelope"

  override def fields: List[Field] = List(
    U32("identifier"),
    Seq16OfByte("extension_body")
  )

}

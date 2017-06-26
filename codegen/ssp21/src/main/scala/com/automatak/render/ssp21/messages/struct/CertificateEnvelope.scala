package com.automatak.render.ssp21.messages.struct

import com.automatak.render.ssp21.messages.{Field, SeqOfByte, Struct}

object CertificateEnvelope extends Struct {

  override def name: String = "CertificateEnvelope"

  override def fields: List[Field] = List(
    SeqOfByte("signature"),
    SeqOfByte("certificate_body")
  )

}

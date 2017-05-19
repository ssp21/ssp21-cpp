package com.automatak.render.ssp21.messages.struct

import com.automatak.render.ssp21.messages.{Field, Seq16OfByte, Seq8OfByte, Struct}

object CertificateEnvelope extends Struct {

  override def name: String = "CertificateEnvelope"

  override def fields: List[Field] = List(
    Seq8OfByte("issuer_id"),
    Seq8OfByte("signature"),
    Seq16OfByte("certificate_body")
  )

}

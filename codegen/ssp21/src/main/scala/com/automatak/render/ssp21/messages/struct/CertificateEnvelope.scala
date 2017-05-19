package com.automatak.render.ssp21.messages.struct

import com.automatak.render.ssp21.messages.{Field, Seq16, Seq8, Struct}

object CertificateEnvelope extends Struct {

  override def name: String = "CertificateEnvelope"

  override def fields: List[Field] = List(
    Seq8("issuer_id"),
    Seq8("signature"),
    Seq16("certificate_body")
  )

}

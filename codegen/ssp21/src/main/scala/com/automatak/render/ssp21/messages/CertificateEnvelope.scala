package com.automatak.render.ssp21.messages

object CertificateEnvelope extends Struct {

  override def name: String = "CertificateEnvelope"

  override def fields: List[Field] = List(
    Seq8("issuer_id"),
    Seq8("signature"),
    Seq16("certificate_body")
  )

}

package com.automatak.render.ssp21.messages

object CertificateEnvelope extends Struct {

  override def name: String = "CertificateEnvelope"

  override def fields: List[Field] = List(
    Seq8("certificate_data"),
    Seq8("issuer_id"),
    U8("signature_algorithm"),
    Seq8("signature")
  )

}

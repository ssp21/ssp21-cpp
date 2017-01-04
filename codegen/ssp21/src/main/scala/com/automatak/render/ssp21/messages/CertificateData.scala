package com.automatak.render.ssp21.messages

object CertificateData extends Struct {

  override def name: String = "CertificateData"

  override def fields: List[Field] = List(
    U32("valid_after"),
    U32("valid_before"),
    Seq8("id"),
    U8("role"),
    U8("key_type"),
    Seq8("public_key")
  )

}

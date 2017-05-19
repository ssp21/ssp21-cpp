package com.automatak.render.ssp21.messages

import com.automatak.render.ssp21.enums.ssp21.PublicKeyType

object CertificateBody extends Struct {

  override def name: String = "CertificateBody"

  override def fields: List[Field] = List(
    U32("serial_number"),
    U32("valid_after"),
    U32("valid_before"),
    Seq8("signing_level"),
    Enum(PublicKeyType),
    Seq8("public_key"),
    Seq8Seq16("extensions", 5)
  )

}

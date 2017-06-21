package com.automatak.render.ssp21.messages.struct

import com.automatak.render.ssp21.enums.ssp21.PublicKeyType
import com.automatak.render.ssp21.messages._

object CertificateBody extends Struct {

  override def name: String = "CertificateBody"

  override def fields: List[Field] = List(
    U64("valid_after"),
    U64("valid_before"),
    U8("signing_level"),
    Enum(PublicKeyType),
    SeqOfByte("public_key"),
    SeqOfStruct("extensions", ExtensionEnvelope, 5)
  )

}

package com.automatak.render.ssp21.messages

import com.automatak.render.ssp21.enums._


object RequestHandshakeBegin extends Message {

  override def name : String = "RequestHandshakeBegin"

  override def fields: List[Field] = List(
      Enum(CryptoFunction()),
      U16("version"),
      Enum(NonceMode()),
      Enum(DHMode()),
      Enum(HashMode()),
      Enum(SessionMode()),
      Enum(CertificateMode()),
      Seq8("ephemeral_public_key"),
      Seq8Seq16("certificates")
  )

}

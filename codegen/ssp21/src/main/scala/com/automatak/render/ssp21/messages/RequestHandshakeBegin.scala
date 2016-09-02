package com.automatak.render.ssp21.messages

import com.automatak.render.ssp21.enums.ssp21._

object RequestHandshakeBegin extends Message {

  override def name: String = "RequestHandshakeBegin"

  def function = CryptoFunction.requestHandshakeBegin

  override def fields: List[Field] = List(
    U16("version"),
    Enum(NonceMode()),
    Enum(DHMode()),
    Enum(HashMode()),
    Enum(SessionMode()),
    Enum(CertificateMode()),
    CommonFields.ephemerialPublicKey,
    CommonFields.certificates
  )

}

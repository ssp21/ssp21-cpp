package com.automatak.render.ssp21.messages

import com.automatak.render.ssp21.enums.ssp21._


object CryptoSpec extends Struct {

  override def name: String = "CryptoSpec"

  override def fields: List[Field] = List(
    Enum(NonceMode()),
    Enum(DHMode()),
    Enum(HandshakeHash()),
    Enum(HandshakeKDF()),
    Enum(HandshakeMAC()),
    Enum(SessionMode())
  )

}

object RequestHandshakeBegin extends Message {

  override def name: String = "RequestHandshakeBegin"

  def function = CryptoFunction.requestHandshakeBegin

  override def fields: List[Field] = List(
    U16("version"),
    StructField("spec", CryptoSpec),
    Enum(CertificateMode()),
    CommonFields.ephemerialPublicKey,
    CommonFields.certificates
  )

}

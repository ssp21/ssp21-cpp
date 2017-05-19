package com.automatak.render.ssp21.messages

import com.automatak.render.ssp21.messages.struct.CertificateEnvelope

object CommonFields {

  val ephemeralPublicKey = Seq8OfByte("ephemeral_public_key")
  val certificates = Seq8OfStruct("certificates", CertificateEnvelope, 6)


}

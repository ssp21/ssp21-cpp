package com.automatak.render.ssp21.messages

import com.automatak.render.ssp21.messages.struct.CertificateEnvelope

object CommonFields {

  val ephemeralPublicKey = SeqOfByte("ephemeral_public_key")
  val certificates = SeqOfStruct("certificates", CertificateEnvelope, 6)


}

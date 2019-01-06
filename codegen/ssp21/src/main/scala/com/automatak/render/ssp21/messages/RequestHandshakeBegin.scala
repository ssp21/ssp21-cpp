/**
 * License TBD
 */
package com.automatak.render.ssp21.messages

import com.automatak.render.ssp21.enums.ssp21._


object CryptoSpec extends Struct {

  override def name: String = "CryptoSpec"

  override def fields: List[Field] = List(
    Enum(NonceMode),
    Enum(HandshakeEphemeral),
    Enum(HandshakeHash),
    Enum(HandshakeKDF),
    Enum(SessionMode)
  )

}

object SessionConstraints extends Struct {

  override def name: String = "SessionConstraints"

  override def fields: List[Field] = List(
    U16("max_nonce"),
    U32("max_session_duration")
  )
}

object RequestHandshakeBegin extends Message {

  override def name: String = "RequestHandshakeBegin"

  def function = CryptoFunction.requestHandshakeBegin

  override def fields: List[Field] = List(
    U16("version"),
    StructField("spec", CryptoSpec),
    StructField("constraints", SessionConstraints),
    Enum(HandshakeMode),
    CommonFields.ephemeralData,
    CommonFields.handshakeData
  )

}

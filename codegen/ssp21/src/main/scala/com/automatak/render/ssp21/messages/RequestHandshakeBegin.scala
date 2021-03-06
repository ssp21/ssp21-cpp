/**
 * License TBD
 */
package com.automatak.render.ssp21.messages

import com.automatak.render.EnumValue
import com.automatak.render.ssp21.enums.ssp21._
import com.automatak.render.ssp21.messages.struct.Version


object CryptoSpec extends Struct {

  override def name: String = "CryptoSpec"

  override def fields: List[Field] = List(
    Enum(HandshakeEphemeral),
    Enum(HandshakeHash),
    Enum(HandshakeKDF),
    Enum(SessionNonceMode),
    Enum(SessionCryptoMode)
  )

  override def public: Boolean = false

}

object SessionConstraints extends Struct {

  override def name: String = "SessionConstraints"

  override def fields: List[Field] = List(
    U16("max_nonce"),
    U32("max_session_duration")
  )

  override def public: Boolean = false

}

object RequestHandshakeBegin extends Message {

  override def name: String = "RequestHandshakeBegin"

  def function: EnumValue = CryptoFunction.requestHandshakeBegin

  override def fields: List[Field] = List(
    StructField("version", Version),
    StructField("spec", CryptoSpec),
    StructField("constraints", SessionConstraints),
    Enum(HandshakeMode),
    CommonFields.modeEphemeral,
    CommonFields.modeData
  )

}

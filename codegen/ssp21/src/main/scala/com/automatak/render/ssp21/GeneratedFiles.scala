/**
  * License TBD
  */
package com.automatak.render.ssp21

import com.automatak.render.cpp.EnumConfig
import com.automatak.render.ssp21.enums.generators.EnumGenerator
import com.automatak.render.ssp21.enums.internal._
import com.automatak.render.ssp21.enums.ssp21._
import com.automatak.render.ssp21.messages._
import com.automatak.render.ssp21.messages.generators.{BitfieldStructGenerator, MessageGenerator, StructGenerator}


object GeneratedFiles {

  def list : List [WriteCppFiles] = internalEnums ::: ssp21Enums ::: bitfiends ::: structs ::: messsages

  val basePath = "ssp21/crypto/gen/"

  private def ssp21Enums = List(
    CryptoFunction(),
    HashMode(),
    NonceMode(),
    DHMode(),
    SessionMode(),
    CertificateMode(),
    HandshakeError()
  ).map(x => EnumConfig(x, true, true)).map(e => EnumGenerator(e, Some(basePath)))

  private def internalEnums = List(
    ParseError(),
    FormatError(),
    CryptoError(),
    Base64DecodeError(),
    ReassemblyResult()
  ).map(x => EnumConfig(x, false, true)).map(e => EnumGenerator(e, Some(basePath)))

  private def bitfiends: List[WriteCppFiles] = List(
    SessionFlags
  ).map(x => BitfieldStructGenerator(x))

  private def structs: List[WriteCppFiles] = List(AuthMetadata).map(x => StructGenerator(x))

  private def messsages: List[WriteCppFiles] = List(
    RequestHandshakeBegin,
    ReplyHandshakeBegin,
    RequestHandshakeAuth,
    ReplyHandshakeAuth,
    ReplyHandshakeError,
    UnconfirmedSessionData
  ).map(m => MessageGenerator(m))

}

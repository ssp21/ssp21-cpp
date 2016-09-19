/**
  * License TBD
  */
package com.automatak.render.ssp21

import com.automatak.render.cpp.EnumConfig
import com.automatak.render.ssp21.enums.generators.EnumGenerator
import com.automatak.render.ssp21.enums.internal.{Base64DecodeError, CryptoError, FormatError, ParseError}
import com.automatak.render.ssp21.enums.ssp21._
import com.automatak.render.ssp21.messages._
import com.automatak.render.ssp21.messages.generators.{BitfieldStructGenerator, MessageGenerator, StructGenerator}


object GeneratedFiles {

  def list : List [WriteCppFiles] = internalEnums ::: ssp21Enums ::: bitfiends ::: structs ::: messsages

  private def ssp21Enums = List(
    CryptoFunction(),
    HashMode(),
    NonceMode(),
    DHMode(),
    SessionMode(),
    CertificateMode(),
    HandshakeError()
  ).map(x => EnumConfig(x, true, true)).map(e => EnumGenerator(e))

  private def internalEnums = List(
    ParseError(),
    FormatError(),
    CryptoError(),
    Base64DecodeError()
  ).map(x => EnumConfig(x, false, true)).map(e => EnumGenerator(e))

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

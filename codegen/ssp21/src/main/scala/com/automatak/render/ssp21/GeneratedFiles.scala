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

  def api: List[WriteCppFiles] = ssp21EnumsAPI
  def impl : List[WriteCppFiles] = ssp21EnumsImpl ::: internalEnums ::: bitfiends ::: structs ::: messsages

  val basePath = "ssp21/crypto/gen/"

  private def ssp21EnumsAPI = List(
    HandshakeHash,
    NonceMode,
    DHMode,
    SessionMode,
    CertificateMode,
    HandshakeError,
    HandshakeKDF,
    HandshakeMAC
  ).map(x => EnumConfig(x, true, true)).map(e => EnumGenerator(e, Some(basePath)))

  private def ssp21EnumsImpl = List(
    CryptoFunction
  ).map(x => EnumConfig(x, true, true)).map(e => EnumGenerator(e, Some(basePath)))

  private def internalEnums = List(
    ParseError,
    FormatError,
    CryptoError,
    Base64DecodeError,
    PEMDecodeError,
    ReassemblyResult
  ).map(x => EnumConfig(x, false, true)).map(e => EnumGenerator(e, Some(basePath)))

  private def bitfiends: List[WriteCppFiles] = List(
    SessionFlags
  ).map(x => BitfieldStructGenerator(x))

  private def structs: List[WriteCppFiles] = List(
    AuthMetadata,
    CryptoSpec,
    SessionConstraints
  ).map(x => StructGenerator(x))

  private def messsages: List[WriteCppFiles] = List(
    RequestHandshakeBegin,
    ReplyHandshakeBegin,
    RequestHandshakeAuth,
    ReplyHandshakeAuth,
    ReplyHandshakeError,
    SessionData
  ).map(m => MessageGenerator(m))

}

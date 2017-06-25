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
import com.automatak.render.ssp21.messages.struct._


object GeneratedFiles {

  def api: List[WriteCppFiles] = ssp21EnumsAPI ::: internalEnumsAPI
  def impl : List[WriteCppFiles] = ssp21EnumsImpl ::: internalEnumsImpl ::: bitfields ::: structs ::: messsages

  val basePath = "ssp21/crypto/gen/"

  private def ssp21EnumsAPI = List(
    HandshakeHash,
    NonceMode,
    DHMode,
    SessionMode,
    CertificateMode,
    HandshakeError,
    HandshakeKDF,
    HandshakeMAC,
    PublicKeyType,
    FileEntryType
  ).map(x => EnumConfig(x, true, true)).map(e => EnumGenerator(e, Some(basePath)))

  private def ssp21EnumsImpl = List(
    CryptoFunction
  ).map(x => EnumConfig(x, true, true)).map(e => EnumGenerator(e, Some(basePath)))

  private def internalEnumsAPI = List(
    Base64DecodeError,
    PEMDecodeError,
    CryptoError
  ).map(x => EnumConfig(x, false, true)).map(e => EnumGenerator(e, Some(basePath)))

  private def internalEnumsImpl = List(
    ParseError,
    FormatError,
    ReassemblyResult
  ).map(x => EnumConfig(x, false, true)).map(e => EnumGenerator(e, Some(basePath)))

  private def bitfields: List[WriteCppFiles] = List(
    SessionFlags
  ).map(x => BitfieldStructGenerator(x))

  private def structs: List[WriteCppFiles] = List(
    AuthMetadata,
    CryptoSpec,
    CertificateEnvelope,
    CertificateBody,
    ExtensionEnvelope,
    SessionConstraints,
    ContainerFile,
    CertificateFileEntry
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

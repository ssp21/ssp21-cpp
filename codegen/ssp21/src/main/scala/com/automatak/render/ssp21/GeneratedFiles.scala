/**
 * License TBD
 */
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

  def api: List[WriteCppFiles] = ssp21EnumsAPI ::: internalEnumsAPI ::: structsAPI
  def impl : List[WriteCppFiles] = ssp21EnumsImpl ::: internalEnumsImpl ::: bitfields ::: structsImpl ::: messages

  private def ssp21EnumsAPI = List(
    HandshakeHash,
    NonceMode,
    HandshakeEphemeral,
    SessionMode,
    HandshakeMode,
    HandshakeError,
    HandshakeKDF,
    PublicKeyType,
    ContainerEntryType
  ).map(x => EnumConfig(x, true, true, true)).map(e => EnumGenerator(e))

  private def ssp21EnumsImpl = List(
    CryptoFunction
  ).map(x => EnumConfig(x, true, true, false)).map(e => EnumGenerator(e))

  private def internalEnumsAPI = List(
    Base64DecodeError,
    PEMDecodeError,
    CryptoError
  ).map(x => EnumConfig(x, false, true, true)).map(e => EnumGenerator(e))

  private def internalEnumsImpl = List(
    ParseError,
    FormatError
  ).map(x => EnumConfig(x, false, true, false)).map(e => EnumGenerator(e))

  private def bitfields: List[WriteCppFiles] = Nil

  private def structsAPI: List[WriteCppFiles] = List(
    CertificateEnvelope,
    CertificateBody,
    CertificateChain,
    ExtensionEnvelope,
    ContainerFile
  ).map(x => StructGenerator(x, true))

  private def structsImpl: List[WriteCppFiles] = List(
    AuthMetadata,
    CryptoSpec,
    SessionConstraints
  ).map(x => StructGenerator(x, false))

  private def messages: List[WriteCppFiles] = List(
    RequestHandshakeBegin,
    ReplyHandshakeBegin,
    ReplyHandshakeError,
    SessionData
  ).map(m => MessageGenerator(m))

}

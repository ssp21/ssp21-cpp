set(ssp21_public_headers
    ./include/ssp21/crypto/BufferTypes.h
    ./include/ssp21/crypto/Chain.h
    ./include/ssp21/crypto/Constants.h
    ./include/ssp21/crypto/Crypto.h
    ./include/ssp21/crypto/CryptoLayerConfig.h
    ./include/ssp21/crypto/CryptoSuite.h
    ./include/ssp21/crypto/CryptoTypedefs.h
    ./include/ssp21/crypto/EnumField.h
    ./include/ssp21/crypto/ICertificateHandler.h
    ./include/ssp21/crypto/ICryptoBackend.h
    ./include/ssp21/crypto/IKeyLookup.h
    ./include/ssp21/crypto/IKeySource.h
    ./include/ssp21/crypto/IMessagePrinter.h
    ./include/ssp21/crypto/IntegerField.h
    ./include/ssp21/crypto/KeyRecord.h
    ./include/ssp21/crypto/SeqByteField.h
    ./include/ssp21/crypto/SeqStructField.h
    ./include/ssp21/crypto/StaticKeys.h
    ./include/ssp21/crypto/Statistics.h
    ./include/ssp21/crypto/VLength.h

    ./include/ssp21/crypto/gen/Base64DecodeError.h
    ./include/ssp21/crypto/gen/CertificateBody.h
    ./include/ssp21/crypto/gen/CertificateChain.h
    ./include/ssp21/crypto/gen/CertificateEnvelope.h
    ./include/ssp21/crypto/gen/ContainerEntryType.h
    ./include/ssp21/crypto/gen/ContainerFile.h
    ./include/ssp21/crypto/gen/CryptoError.h
    ./include/ssp21/crypto/gen/ExtensionEnvelope.h
    ./include/ssp21/crypto/gen/FormatError.h
    ./include/ssp21/crypto/gen/HandshakeEphemeral.h
    ./include/ssp21/crypto/gen/HandshakeError.h
    ./include/ssp21/crypto/gen/HandshakeHash.h
    ./include/ssp21/crypto/gen/HandshakeKDF.h
    ./include/ssp21/crypto/gen/HandshakeMode.h
    ./include/ssp21/crypto/gen/NonceMode.h
    ./include/ssp21/crypto/gen/ParseError.h
    ./include/ssp21/crypto/gen/PEMDecodeError.h
    ./include/ssp21/crypto/gen/PublicKeyType.h
    ./include/ssp21/crypto/gen/SessionMode.h

    ./include/ssp21/link/Addresses.h
    ./include/ssp21/link/LinkConstants.h

    ./include/ssp21/stack/Factory.h
    ./include/ssp21/stack/ILowerLayer.h
    ./include/ssp21/stack/IStack.h
    ./include/ssp21/stack/IUpperLayer.h
    ./include/ssp21/stack/LogLevels.h

    ./include/ssp21/util/ConsolePrettyPrinter.h
    ./include/ssp21/util/ErrorCategory.h
    ./include/ssp21/util/Exception.h
    ./include/ssp21/util/ICollection.h
    ./include/ssp21/util/PrintHex.h
    ./include/ssp21/util/SecureDynamicBuffer.h
    ./include/ssp21/util/SecureFile.h
    ./include/ssp21/util/SequenceTypes.h
    ./include/ssp21/util/SerializationUtils.h
    ./include/ssp21/util/StringUtil.h
)

set(ssp21_private_headers
    ./src/IFrameWriter.h
    ./src/IWritable.h

    ./src/crypto/Algorithms.h
    ./src/crypto/AlgorithmSet.h
    ./src/crypto/CryptoLayer.h
    ./src/crypto/FlagsPrinting.h
    ./src/crypto/FormatResult.h
    ./src/crypto/HandshakeHasher.h
    ./src/crypto/IInitiatorHandshake.h
    ./src/crypto/IMessage.h
    ./src/crypto/IMessageHandler.h
    ./src/crypto/IndustrialCertificateHandler.h
    ./src/crypto/Initiator.h
    ./src/crypto/InitiatorHandshakes.h
    ./src/crypto/InitiatorHandshakeStates.h
    ./src/crypto/IResponderHandshake.h
    ./src/crypto/ISessionMode.h
    ./src/crypto/LogMessagePrinter.h
    ./src/crypto/MessageFormatter.h
    ./src/crypto/MessageOnlyFrameWriter.h
    ./src/crypto/MessageParser.h
    ./src/crypto/MessagePrinting.h
    ./src/crypto/Nonce.h
    ./src/crypto/NonceFunctions.h
    ./src/crypto/PresharedKeyCertificateHandler.h
    ./src/crypto/PublicKeyInitiatorHandshake.h
    ./src/crypto/PublicKeyResponderHandshake.h
    ./src/crypto/QKDInitiatorHandshake.h
    ./src/crypto/QKDResponderHandshake.h
    ./src/crypto/Responder.h
    ./src/crypto/ResponderHandshakes.h
    ./src/crypto/Session.h
    ./src/crypto/SessionModes.h
    ./src/crypto/Sessions.h
    ./src/crypto/SharedSecretInitiatorHandshake.h
    ./src/crypto/SharedSecretResponderHandshake.h
    ./src/crypto/TripleDH.h
    ./src/crypto/TruncatedMacSessionMode.h
    ./src/crypto/TxState.h

    ./src/crypto/gen/AuthMetadata.h
    ./src/crypto/gen/CryptoSpec.h
    ./src/crypto/gen/Function.h
    ./src/crypto/gen/ReplyHandshakeBegin.h
    ./src/crypto/gen/ReplyHandshakeError.h
    ./src/crypto/gen/RequestHandshakeBegin.h
    ./src/crypto/gen/SessionConstraints.h
    ./src/crypto/gen/SessionData.h

    ./src/link/CastagnoliCRC32.h
    ./src/link/LinkFrameWriter.h
    ./src/link/LinkLayer.h
    ./src/link/LinkParser.h

    ./src/stack/LinkCryptoStack.h
)

set(ssp21_srcs
    ./src/IFrameWriter.cpp

    ./src/crypto/Algorithms.cpp
    ./src/crypto/BufferTypes.cpp
    ./src/crypto/Chain.cpp
    ./src/crypto/Crypto.cpp
    ./src/crypto/CryptoLayer.cpp
    ./src/crypto/FlagsPrinting.cpp
    ./src/crypto/HandshakeHasher.cpp
    ./src/crypto/ICertificateHandler.cpp
    ./src/crypto/IndustrialCertificateHandler.cpp
    ./src/crypto/Initiator.cpp
    ./src/crypto/InitiatorHandshakes.cpp
    ./src/crypto/InitiatorHandshakeStates.cpp
    ./src/crypto/KeyRecord.cpp
    ./src/crypto/LogLevels.cpp
    ./src/crypto/LogMessagePrinter.cpp
    ./src/crypto/PublicKeyInitiatorHandshake.cpp
    ./src/crypto/PublicKeyResponderHandshake.cpp
    ./src/crypto/QKDInitiatorHandshake.cpp
    ./src/crypto/QKDResponderHandshake.cpp
    ./src/crypto/Responder.cpp
    ./src/crypto/ResponderHandshakes.cpp
    ./src/crypto/Session.cpp
    ./src/crypto/SessionModes.cpp
    ./src/crypto/Sessions.cpp
    ./src/crypto/SharedSecretInitiatorHandshake.cpp
    ./src/crypto/SharedSecretResponderHandshake.cpp
    ./src/crypto/TripleDH.cpp
    ./src/crypto/TruncatedMacSessionMode.cpp
    ./src/crypto/VLength.cpp

    ./src/crypto/gen/AuthMetadata.cpp
    ./src/crypto/gen/Base64DecodeError.cpp
    ./src/crypto/gen/CertificateBody.cpp
    ./src/crypto/gen/CertificateChain.cpp
    ./src/crypto/gen/CertificateEnvelope.cpp
    ./src/crypto/gen/ContainerEntryType.cpp
    ./src/crypto/gen/ContainerFile.cpp
    ./src/crypto/gen/CryptoError.cpp
    ./src/crypto/gen/CryptoSpec.cpp
    ./src/crypto/gen/ExtensionEnvelope.cpp
    ./src/crypto/gen/FormatError.cpp
    ./src/crypto/gen/Function.cpp
    ./src/crypto/gen/HandshakeEphemeral.cpp
    ./src/crypto/gen/HandshakeError.cpp
    ./src/crypto/gen/HandshakeHash.cpp
    ./src/crypto/gen/HandshakeKDF.cpp
    ./src/crypto/gen/HandshakeMode.cpp
    ./src/crypto/gen/NonceMode.cpp
    ./src/crypto/gen/ParseError.cpp
    ./src/crypto/gen/PEMDecodeError.cpp
    ./src/crypto/gen/PublicKeyType.cpp
    ./src/crypto/gen/ReplyHandshakeBegin.cpp
    ./src/crypto/gen/ReplyHandshakeError.cpp
    ./src/crypto/gen/RequestHandshakeBegin.cpp
    ./src/crypto/gen/SessionConstraints.cpp
    ./src/crypto/gen/SessionData.cpp
    ./src/crypto/gen/SessionMode.cpp

    ./src/link/CastagnoliCRC32.cpp
    ./src/link/LinkFrameWriter.cpp
    ./src/link/LinkLayer.cpp
    ./src/link/LinkParser.cpp

    ./src/stack/Factory.cpp

    ./src/util/ConsolePrettyPrinter.cpp
    ./src/util/SecureFile.cpp
)

add_library(ssp21 ${ssp21_public_headers} ${ssp21_private_headers} ${ssp21_srcs})
target_include_directories(ssp21
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE ./src
)
target_link_libraries(ssp21 PUBLIC openpal)

install(TARGETS ssp21 EXPORT Ssp21Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(DIRECTORY ./include/ssp21 DESTINATION include)

add_subdirectory(./tests)



#include "catch.hpp"

#include "ssp21/msg/RequestHandshakeBegin.h"
#include "ssp21/LogLinePrinter.h"

#include "testlib/BufferHelpers.h"
#include "testlib/HexConversions.h"

#include "openpal/container/StaticBuffer.h"

#include "mocks/MockLogger.h"

#define SUITE(name) "RequestHandshakeBeginTestSuite - " name

using namespace ssp21;
using namespace openpal;

TEST_CASE(SUITE("returns error on empty message"))
{		
	RequestHandshakeBegin msg;
	auto input = RSlice::empty_slice();
	REQUIRE(msg.read_msg(input) == ParseError::insufficient_bytes);
}

TEST_CASE(SUITE("returns error on undefined enum"))
{
	RequestHandshakeBegin msg;

	Hex hex("DD");

	auto input = hex.as_rslice();
	auto err = msg.read_msg(input);
	REQUIRE(err == ParseError::undefined_enum);
}

TEST_CASE(SUITE("returns error on unexpected function"))
{
	RequestHandshakeBegin msg;

	Hex hex("03");

	auto input = hex.as_rslice();
	auto err = msg.read_msg(input);
	REQUIRE(err == ParseError::unexpected_function);
}

TEST_CASE(SUITE("returns error if too little data"))
{
	RequestHandshakeBegin msg;

	Hex hex("00");

	auto input = hex.as_rslice();
	auto err = msg.read_msg(input);
	REQUIRE(err == ParseError::insufficient_bytes);
}

TEST_CASE(SUITE("successfully parses message"))
{
	RequestHandshakeBegin msg;

	Hex hex("00 D2 D1 00 00 00 00 00 03 AA AA AA 01 02 00 BB BB");

	auto input = hex.as_rslice();
	auto err = msg.read_msg(input);
	REQUIRE(!any(err));
	REQUIRE(msg.version == 0xD1D2);
	REQUIRE(msg.nonce_mode == NonceMode::increment_last_rx);
	REQUIRE(msg.dh_mode == DHMode::x25519);
	REQUIRE(msg.hash_mode == HashMode::sha256);
	REQUIRE(msg.session_mode == SessionMode::hmac_sha256_16);
	REQUIRE(msg.certificate_mode == CertificateMode::preshared_keys);

	REQUIRE(to_hex(msg.ephemeral_public_key) == "AA AA AA");
	
	REQUIRE(msg.certificates.count() == 1);
	Seq16 cert;
	REQUIRE(msg.certificates.read(0, cert));
	REQUIRE(to_hex(cert) == "BB BB");
}

TEST_CASE(SUITE("pretty prints message"))
{
	
	Hex publicKey("CA FE");
	Hex cert1("AA AA AA AA AA AA AA AA AA AA AA AA AA AA AA AA AA AA AA AA AA AA AA AA AA AA AA AA AA AA AA AA");
	Hex cert2("CC DD");

	RequestHandshakeBegin msg(
		7,
		NonceMode::greater_than_last_rx,
		DHMode::x25519,
		HashMode::sha256,
		SessionMode::hmac_sha256_16,
		CertificateMode::preshared_keys,
		Seq8(publicKey)
	);

	REQUIRE(msg.certificates.push(cert1));
	REQUIRE(msg.certificates.push(cert2));


	MockLogger log("log");
	LogLinePrinter printer(log.root.logger, levels::info, 16);

	msg.print(printer);
		
	log.validate(
		"version: 7",
		"nonce_mode: greater_than_last_rx",
		"dh_mode: x25519",
		"hash_mode: sha256",
		"session_mode: hmac_sha256_16",
		"certificate_mode: preshared_keys",
		"ephemeral_public_key (length = 2)",
		"CA:FE",
		"certificates (count = 2)",
		"#1 (length = 32)",
		"AA:AA:AA:AA:AA:AA:AA:AA:AA:AA:AA:AA:AA:AA:AA:AA",	
		"AA:AA:AA:AA:AA:AA:AA:AA:AA:AA:AA:AA:AA:AA:AA:AA",
		"#2 (length = 2)",
		"CC:DD"
	);
		
}

TEST_CASE(SUITE("rejects unknown enum"))
{
	RequestHandshakeBegin msg;

	Hex hex("00 D2 D1 00 CC 00 00 00 03 AA AA AA 01 02 00 BB BB");

	auto input = hex.as_rslice();
	auto err = msg.read_msg(input);
	REQUIRE(err == ParseError::undefined_enum);
}

TEST_CASE(SUITE("rejects trailing data"))
{
	RequestHandshakeBegin msg;

	Hex hex("00 D2 D1 00 00 00 00 00 03 AA AA AA 01 02 00 BB BB FF FF FF");

	auto input = hex.as_rslice();
	auto err = msg.read_msg(input);
	REQUIRE(err == ParseError::too_many_bytes);
}

TEST_CASE(SUITE("formats default value"))
{
	openpal::StaticBuffer<RequestHandshakeBegin::min_size_bytes> buffer;
	RequestHandshakeBegin msg;
	auto dest = buffer.as_wslice();
	auto res = msg.write_msg(dest);
	
	REQUIRE(!res.is_error());
	REQUIRE(to_hex(res.written) == "00 00 00 FF FF FF FF FF 00 00");	
}

TEST_CASE(SUITE("returns error if insufficient buffer space"))
{
	openpal::StaticBuffer<RequestHandshakeBegin::min_size_bytes - 1> buffer;
	RequestHandshakeBegin msg;
	auto dest = buffer.as_wslice();
	REQUIRE(msg.write_msg(dest).err == FormatError::insufficient_space);
}
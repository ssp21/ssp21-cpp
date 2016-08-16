

#include "catch.hpp"

#include "ssp21/msg/RequestHandshakeBegin.h"

#include "testlib\BufferHelpers.h"
#include "testlib\HexConversions.h"

#define SUITE(name) "RequestHandshakeBeginTestSuite - " name

using namespace ssp21;
using namespace openpal;

TEST_CASE(SUITE("returns error on empty message"))
{		
	RequestHandshakeBegin msg;
	REQUIRE(msg.read(RSlice::empty_slice()) == ParseError::insufficient_bytes);
}

TEST_CASE(SUITE("returns error on undefined enum"))
{
	RequestHandshakeBegin msg;

	HexSequence hex("DD");

	auto input = hex.as_rslice();
	auto err = msg.read(input);
	REQUIRE(err == ParseError::undefined_enum);
}

TEST_CASE(SUITE("returns error on unexpected function"))
{
	RequestHandshakeBegin msg;

	HexSequence hex("03");

	auto input = hex.as_rslice();
	auto err = msg.read(input);
	REQUIRE(err == ParseError::unexpected_function);
}

TEST_CASE(SUITE("returns error if too little data"))
{
	RequestHandshakeBegin msg;

	HexSequence hex("00");

	auto input = hex.as_rslice();
	auto err = msg.read(input);
	REQUIRE(err == ParseError::insufficient_bytes);
}

TEST_CASE(SUITE("successfully parses message"))
{
	RequestHandshakeBegin msg;

	HexSequence hex("00 D2 D1 00 00 00 00 00 03 AA AA AA 01 02 00 BB BB");

	auto input = hex.as_rslice();
	auto err = msg.read(input);
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


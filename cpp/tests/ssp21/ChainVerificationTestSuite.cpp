
#include "catch.hpp"

#include "ssp21/crypto/Chain.h"
#include "ssp21/crypto/gen/CertificateChain.h"

#include "mocks/CryptoFixture.h"

#include "testlib/Hex.h"
#include "testlib/HexConversions.h"

using namespace ssp21;
using namespace openpal;

#define SUITE(name) "ChainVerificationTestSuite - " name

TEST_CASE(SUITE("successfully verifies single certificate"))
{
	CryptoFixture fix;

	const Hex anchor_public_key(repeat_hex(0xFF, consts::crypto::x25519_key_length));

	const CertificateBody anchor_cert(
		0,
		0xFFFFFFFF,
		0,
		PublicKeyType::X25519,
		anchor_public_key.as_rslice()
	);

	CertificateChain chain;
	//chain.certificates.push()

	//Chain::verify(anchor, )
}


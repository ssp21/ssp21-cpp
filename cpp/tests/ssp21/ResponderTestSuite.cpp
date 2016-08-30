

#include "catch.hpp"

#include "ssp21/crypto/Responder.h"

#include "openpal/container/StaticBuffer.h"

#include "testlib/Hex.h"
#include "testlib/MockExecutor.h"

#include "mocks/MockLogHandler.h"
#include "mocks/MockLowerLayer.h"
#include "mocks/HexMessageBuilders.h"
#include "mocks/MakeUnique.h"
#include "mocks/MockCryptoBackend.h"

#define SUITE(name) "ResponderTestSuite - " name

using namespace ssp21;
using namespace openpal;

class ResponderFixture
{
private:

	struct Keys
	{
		Keys(BufferType key_type) :
			local_kp(std::make_unique<KeyPair>()),
			remote_static_key(std::make_unique<PublicKey>())
		{			
			init_key(local_kp->private_key, key_type);
			init_key(local_kp->public_key, key_type);
			init_key(*remote_static_key, key_type);
		}
		
		static void init_key(BufferBase& buffer, BufferType key_type)
		{
			buffer.get_write_slice().set_all_to(0xFF);
			buffer.set_type(key_type);
		}

		std::unique_ptr<KeyPair> local_kp;
		std::unique_ptr<PublicKey> remote_static_key;
	};


public:

    ResponderFixture(BufferType key_type = BufferType::x25519_key, const Responder::Config& config = Responder::Config()) :
        keys(key_type),
		log("responder"),
        exe(),
        lower(),
        responder(config, std::move(keys.local_kp), std::move(keys.remote_static_key), log.root.logger, exe, lower)
    {
	
	}

private:

	Keys keys;

public:
	
    MockLogHandler log;
    MockExecutor exe;
    MockLowerLayer lower;
    Responder responder;
};

TEST_CASE(SUITE("responds to malformed REQUEST_HANDSHAKE_BEGIN with bad_message_format"))
{	
	ResponderFixture fix;

    fix.responder.on_open();

    // request handshake begin (function only)
	fix.lower.enqueue_message(Addresses(1, 10), hex::func_to_hex(Function::request_handshake_begin));
    fix.responder.on_rx_ready();

	auto err = hex::reply_handshake_error(HandshakeError::bad_message_format);
    
    REQUIRE(fix.lower.pop_tx_message() == err);
	REQUIRE(MockCryptoBackend::instance.counters.all_zero());
}

TEST_CASE(SUITE("responds to REQUEST_HANDSHAKE_BEGIN with REPLY_HANDSHAKE_BEGIN"))
{
	ResponderFixture fix;

	fix.responder.on_open();

	auto request = hex::request_handshake_begin(
		0,
		NonceMode::increment_last_rx,
		DHMode::x25519,
		HashMode::sha256,
		SessionMode::hmac_sha256_16,
		CertificateMode::preshared_keys,
		hex::repeat(0xFF, consts::x25519_key_length)		
	);
	
	fix.lower.enqueue_message(Addresses(1, 10), request);
	fix.responder.on_rx_ready();

	auto& counters = MockCryptoBackend::instance.counters;
	
	REQUIRE(counters.num_hash_sha256 == 2);			// two hashes for the chaining key
	REQUIRE(counters.num_gen_keypair_x25519 == 1);	// one ephemeral key pair generation
	REQUIRE(counters.num_dh_x25519 == 3);			// 3 DH operations to get the authentication key
	REQUIRE(counters.num_hmac_sha256 == 3);			// 3 hmacs during the HKDF

	auto reply = hex::reply_handshake_begin(hex::repeat(0xFF, consts::x25519_key_length));
	
	REQUIRE(fix.lower.pop_tx_message() == reply);
}

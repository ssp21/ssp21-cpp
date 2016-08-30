

#include "catch.hpp"

#include "ssp21/crypto/Responder.h"

#include "openpal/container/StaticBuffer.h"

#include "testlib/Hex.h"
#include "testlib/MockExecutor.h"

#include "mocks/MockLogHandler.h"
#include "mocks/MockLowerLayer.h"
#include "mocks/HexMessageBuilders.h"

#define SUITE(name) "ResponderTestSuite - " name

using namespace ssp21;
using namespace openpal;

class ResponderFixture
{
private:

	struct Keys
	{
		Keys() :
			local_kp(std::make_unique<KeyPair>()),
			remote_static_key(std::make_unique<PublicKey>())
		{			
			Crypto::gen_keypair_x25519(*local_kp);			
			remote_static_key->get_write_slice().set_all_to(0xFF);
			remote_static_key->set_type(BufferType::x25519_key);
		}

		std::unique_ptr<KeyPair> local_kp;
		std::unique_ptr<PublicKey> remote_static_key;
	};


public:

    ResponderFixture(const Responder::Config& config = Responder::Config()) :
        keys(),
		log("responder"),
        exe(),
        lower(),
        responder(config, std::move(keys.local_kp), std::move(keys.remote_static_key), exe, log.root.logger, lower)
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
	fix.lower.enqueue_message(Addresses(5, 5), hex::func_to_hex(Function::request_handshake_begin));
    fix.responder.on_rx_ready();

	auto err = hex::reply_handshake_error(HandshakeError::bad_message_format);
    
    REQUIRE(fix.lower.pop_tx_message() == err);
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
	
	fix.lower.enqueue_message(Addresses(5, 5), request);
	fix.responder.on_rx_ready();

	auto reply = hex::reply_handshake_begin(hex::repeat(0xFF, consts::x25519_key_length));
	
	REQUIRE(fix.lower.pop_tx_message() == reply);
}

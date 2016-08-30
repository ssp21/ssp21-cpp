

#include "catch.hpp"

#include "ssp21/crypto/Responder.h"

#include "openpal/container/StaticBuffer.h"

#include "testlib/Hex.h"
#include "testlib/MockExecutor.h"

#include "mocks/MockLogHandler.h"
#include "mocks/MockLowerLayer.h"

#define SUITE(name) "ResponderTestSuite - " name

using namespace ssp21;
using namespace openpal;

struct ResponderFixture
{
    ResponderFixture(std::unique_ptr<KeyPair> local_kp, std::unique_ptr<PublicKey> remote_static_key, const Responder::Config& config = Responder::Config()) :
        log("responder"),
        exe(),
        lower(),
        responder(config, std::move(local_kp), std::move(remote_static_key), exe, log.root.logger, lower)
    {
	
	}
	
    MockLogHandler log;
    MockExecutor exe;
    MockLowerLayer lower;
    Responder responder;
};

TEST_CASE(SUITE("responds to malformed REQUEST_HANDSHAKE_BEGIN with bad_message_format"))
{
	std::unique_ptr<KeyPair> local_kp;
	Crypto::gen_keypair_x25519(*local_kp);

	std::unique_ptr<PublicKey> remote_key;
	remote_key->get_write_slice().set_all_to(0xFF);
	remote_key->set_type(BufferType::x25519_key);

	ResponderFixture fix(std::move(local_kp), std::move(remote_key));

    fix.responder.on_open();

    // request handshake begin (function only)
    fix.lower.enqueue_message(Addresses(5, 5), "00");
    fix.responder.on_rx_ready();

    // ReplyHandshakeError w/ error = bad message format
    REQUIRE(fix.lower.pop_tx_message() == "04 00");
}

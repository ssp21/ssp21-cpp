

#ifndef RESPONDER_FIXTURE_H
#define RESPONDER_FIXTURE_H

#include "ssp21/crypto/BufferTypes.h"
#include "ssp21/crypto/Responder.h"

#include "testlib/MockExecutor.h"

#include "../mocks/MockLogHandler.h"
#include "../mocks/MockLowerLayer.h"
#include "../mocks/MockUpperLayer.h"
#include "../mocks/MockCryptoBackend.h"
#include "../mocks/HexMessageBuilders.h"

#include <memory>

namespace ssp21
{

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
                buffer.as_wseq().set_all_to(0xFF);
                buffer.set_type(key_type);
            }

            std::unique_ptr<KeyPair> local_kp;
            std::unique_ptr<PublicKey> remote_static_key;
        };


    public:

        ResponderFixture(BufferType key_type = BufferType::x25519_key, const Responder::Config& config = Responder::Config()) :
            keys(key_type),
            log("responder"),
            exe(openpal::MockExecutor::Create()),
            lower(),
            responder(config, std::move(keys.local_kp), std::move(keys.remote_static_key), log.logger, exe, lower),
            upper(responder)
        {
            MockCryptoBackend::instance.clear_actions();
            responder.set_upper_layer(upper);
        }

        void set_tx_ready()
        {
            lower.set_tx_ready();
            responder.on_tx_ready();
        }

    private:

        Keys keys;

    public:

        ssp21::MockLogHandler log;
        std::shared_ptr<openpal::MockExecutor> exe;
        MockLowerLayer lower;
        Responder responder;
        MockUpperLayer upper;
    };

}

#endif




#ifndef SSP21_CRYPTOLAYERFIXTURE_H
#define SSP21_CRYPTOLAYERFIXTURE_H


#include "ssp21/crypto/Responder.h"
#include "ssp21/crypto/Initiator.h"

#include "testlib/MockExecutor.h"

#include "../mocks/MockLogHandler.h"
#include "../mocks/MockLowerLayer.h"
#include "../mocks/MockUpperLayer.h"
#include "../mocks/MockCryptoBackend.h"
#include "../mocks/HexMessageBuilders.h"
#include "../mocks/MockFrameWriter.h"

namespace ssp21
{

    struct MockKeys
    {
        MockKeys(BufferType key_type) :
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

    class CryptoLayerFixture
    {

    public:

        CryptoLayerFixture(
            const Session::Config& session_config = Session::Config(),
            uint16_t max_message_size = consts::link::max_config_payload_size
        ) :
            keys(BufferType::x25519_key),
            log("responder"),
            exe(openpal::MockExecutor::Create())
        {
            MockCryptoBackend::instance.clear_actions();
        }

        static std::shared_ptr<IFrameWriter> get_frame_writer(const openpal::Logger& logger, uint16_t max_message_size)
        {
            return std::make_shared<MockFrameWriter>(logger, max_message_size);
        }

    protected:

        MockKeys keys;

    public:

        ssp21::MockLogHandler log;
        const std::shared_ptr<openpal::MockExecutor> exe;
        MockLowerLayer lower;
        MockUpperLayer upper;

    };

    struct ResponderFixture : public CryptoLayerFixture
    {
        ResponderFixture(
            const Session::Config& session_config = Session::Config(),
            const Responder::Config& config = Responder::Config(),
            uint16_t max_message_size = consts::link::max_config_payload_size
        ) :
            CryptoLayerFixture(session_config, max_message_size),
            responder(config, session_config, this->log.logger, get_frame_writer(this->log.logger, max_message_size), this->exe, std::move(this->keys.local_kp), std::move(this->keys.remote_static_key))
        {
            lower.bind_upper(responder);
            upper.bind_lower(responder);
            responder.bind_layers(lower, upper);
        }

        void set_tx_ready()
        {
            lower.set_tx_ready(true);
            responder.on_tx_ready();
        }

        Responder responder;
    };

    struct InitiatorFixture : public CryptoLayerFixture
    {
        InitiatorFixture(
            const Session::Config& session_config = Session::Config(),
            const Initiator::Config& config = Initiator::Config(),
            uint16_t max_message_size = consts::link::max_config_payload_size
        ) :
            CryptoLayerFixture(session_config, max_message_size),
            initiator(config, session_config, this->log.logger, get_frame_writer(this->log.logger, max_message_size), this->exe, std::move(this->keys.local_kp), std::move(this->keys.remote_static_key))
        {
            lower.bind_upper(initiator);
            upper.bind_lower(initiator);
            initiator.bind_layers(lower, upper);
        }

        Initiator initiator;
    };

}

#endif


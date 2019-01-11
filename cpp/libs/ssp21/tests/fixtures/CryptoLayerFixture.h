

#ifndef SSP21_CRYPTOLAYERFIXTURE_H
#define SSP21_CRYPTOLAYERFIXTURE_H

#include "crypto/Responder.h"
#include "crypto/Initiator.h"
#include "crypto/MessageOnlyFrameWriter.h"
#include "crypto/ResponderHandshakes.h"
#include "crypto/InitiatorHandshakes.h"

#include "log4cpp/MockLogHandler.h"
#include "mocks/MockLowerLayer.h"
#include "mocks/MockUpperLayer.h"
#include "mocks/CryptoFixture.h"
#include "mocks/HexMessageBuilders.h"

#include "exe4cpp/MockExecutor.h"

namespace ssp21
{
    class CryptoLayerFixture : public CryptoFixture
    {

    public:

        CryptoLayerFixture(
            const SessionConfig& session_config = SessionConfig(),
            uint16_t max_message_size = consts::link::max_config_payload_size
        ) :
            log("responder"),
            exe(std::make_shared<exe4cpp::MockExecutor>())
        {

        }

        static std::shared_ptr<IFrameWriter> get_frame_writer(const log4cpp::Logger& logger, uint16_t max_message_size)
        {
            return std::make_shared<MessageOnlyFrameWriter>(logger, max_message_size);
        }

    protected:

        static StaticKeys get_keys()
        {
            const auto local_pub = std::make_shared<PublicKey>();
            const auto local_priv = std::make_shared<PrivateKey>();

            init_key(*local_pub);
            init_key(*local_priv);

            return StaticKeys(local_pub, local_priv);
        }

        static std::shared_ptr<ICertificateHandler> get_certificate_handler()
        {
            const auto local_pub = std::make_shared<PublicKey>();

            init_key(*local_pub);

            return ICertificateHandler::preshared_key(local_pub);
        }

        static void init_key(BufferBase& buffer)
        {
            buffer.as_wseq().set_all_to(0xFF);
            buffer.set_type(BufferType::x25519_key);
        }

    public:

        log4cpp::MockLogHandler log;
        const std::shared_ptr<exe4cpp::MockExecutor> exe;
        MockLowerLayer lower;
        MockUpperLayer upper;

    };

    struct ResponderFixture : public CryptoLayerFixture
    {
        ResponderFixture(
            const ResponderConfig& config = ResponderConfig(),
            uint16_t max_message_size = consts::link::max_config_payload_size
        ) :
            CryptoLayerFixture(config.session, max_message_size),
            responder(
                config,
                this->log.logger,
                get_frame_writer(this->log.logger, max_message_size),
                this->exe,
                ResponderHandshakes::public_key_mode(this->log.logger, get_keys(), get_certificate_handler())
            )
        {
            lower.bind_upper(responder);
            upper.bind_lower(responder);
            responder.bind(lower, upper);
        }

        void set_tx_ready()
        {
            lower.set_tx_ready(true);
            responder.on_lower_tx_ready();
        }

        Responder responder;
    };

    struct InitiatorFixture : public CryptoLayerFixture
    {
        InitiatorFixture(
            const InitiatorConfig& config = InitiatorConfig(),
            const CryptoSuite& crypto_suite = CryptoSuite(),
            uint16_t max_message_size = consts::link::max_config_payload_size
        ) :
            CryptoLayerFixture(config.session, max_message_size),
            initiator(
                config,
                this->log.logger,
                get_frame_writer(this->log.logger, max_message_size),
                this->exe,
                InitiatorHandshakes::public_key_mode(
                    this->log.logger,
                    crypto_suite,
                    get_keys(),
                    get_certificate_handler()
                )
            )
        {
            lower.bind_upper(initiator);
            upper.bind_lower(initiator);
            initiator.bind(lower, upper);
        }

        Initiator initiator;
    };

}

#endif


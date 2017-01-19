#ifndef SSP21_INTEGRATION_FIXTURE_H
#define SSP21_INTEGRATION_FIXTURE_H

#include "../mocks/LowerLayer.h"
#include "../mocks/UpperLayer.h"

#include "testlib/MockExecutor.h"
#include "testlib/MockLogHandler.h"

#include "ssp21/crypto/Initiator.h"
#include "ssp21/crypto/Responder.h"
#include "ssp21/crypto/MessageOnlyFrameWriter.h"
#include "ssp21/MakeUnique.h"

namespace ssp21
{

    class IntegrationFixture
    {

    public:

        IntegrationFixture() :
            exe(std::make_shared<openpal::MockExecutor>()),
            initiator_lower(exe),
            responder_lower(exe)
        {
            // we need to first perform some key derivation so we can inject private keys, and share public keys
            auto kp_responder = std::make_unique<KeyPair>();
            auto kp_initiator = std::make_unique<KeyPair>();

            Crypto::gen_keypair_x25519(*kp_responder);
            Crypto::gen_keypair_x25519(*kp_initiator);

            // make copies of the public keys
            auto responder_pub_copy = std::make_unique<PublicKey>(kp_responder->public_key);
            auto initiator_pub_copy = std::make_unique<PublicKey>(kp_initiator->public_key);

            initiator = std::make_unique<Initiator>(Initiator::Config(), Session::Config(), log.logger, std::make_shared<MessageOnlyFrameWriter>(), exe, std::move(kp_initiator), std::move(responder_pub_copy));
            responder = std::make_unique<Responder>(Responder::Config(), Session::Config(), log.logger, std::make_shared<MessageOnlyFrameWriter>(), exe, std::move(kp_responder), std::move(initiator_pub_copy));

            // wire the lower layers together
            initiator_lower.configure(*initiator, responder_lower);
            responder_lower.configure(*responder, initiator_lower);

            // wire the upper layers
            initiator_upper.configure(*initiator);
            responder_upper.configure(*responder);

            // wire the initiator and responder
            initiator->bind_layers(initiator_lower, initiator_upper);
            responder->bind_layers(responder_lower, responder_upper);
        }


        const std::shared_ptr<openpal::MockExecutor> exe;
        openpal::MockLogHandler log;

        LowerLayer initiator_lower;
        LowerLayer responder_lower;

        UpperLayer initiator_upper;
        UpperLayer responder_upper;

        std::unique_ptr<Initiator> initiator;
        std::unique_ptr<Responder> responder;
    };

}

#endif

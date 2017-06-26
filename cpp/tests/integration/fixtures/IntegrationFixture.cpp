#include "IntegrationFixture.h"

#include "ssp21/crypto/Crypto.h"
#include "ssp21/stack/Factory.h"

namespace ssp21
{

    IntegrationFixture::IntegrationFixture() :
        exe(std::make_shared<openpal::MockExecutor>()),
        ilog("initiator"),
        rlog("responder"),
        initiator_lower(exe),
        responder_lower(exe),
        stacks(preshared_key_stacks(rlog.logger, ilog.logger, exe))
    {
        this->wire();
    }

    IntegrationFixture::Stacks IntegrationFixture::preshared_key_stacks(openpal::Logger rlogger, openpal::Logger ilogger, std::shared_ptr<openpal::IExecutor> exe)
    {
        const auto keys = generate_random_keys();

        const auto initiator = Factory::initiator(
                                   Addresses(1, 10),
                                   InitiatorConfig(),
                                   rlogger,
                                   exe,
                                   keys.initiator,
                                   ICertificateHandler::preshared_key(keys.responder.public_key)
                               );

        const auto responder = Factory::responder(
                                   Addresses(10, 1),
                                   ResponderConfig(),
                                   ilogger,
                                   exe,
                                   keys.responder,
                                   ICertificateHandler::preshared_key(keys.initiator.public_key)
                               );

        return Stacks{ initiator, responder };
    }

    IntegrationFixture::Stacks IntegrationFixture::certificate_stacks(openpal::Logger rlogger, openpal::Logger ilogger, std::shared_ptr<openpal::IExecutor> exe)
    {
        const auto keys = generate_random_keys();

        // TODO - we need to generate certificates here

        const auto initiator = Factory::initiator(
                                   Addresses(1, 10),
                                   InitiatorConfig(),
                                   rlogger,
                                   exe,
                                   keys.initiator,
                                   ICertificateHandler::preshared_key(keys.responder.public_key)
                               );

        const auto responder = Factory::responder(
                                   Addresses(10, 1),
                                   ResponderConfig(),
                                   ilogger,
                                   exe,
                                   keys.responder,
                                   ICertificateHandler::preshared_key(keys.initiator.public_key)
                               );

        return Stacks{ initiator, responder };
    }

    IntegrationFixture::Keys IntegrationFixture::generate_random_keys()
    {
        // we need to first perform some key derivation
        KeyPair kp_responder;
        KeyPair kp_initiator;

        Crypto::gen_keypair_x25519(kp_responder);
        Crypto::gen_keypair_x25519(kp_initiator);

        // make copies of the all the keys on the heap

        return Keys
        {
            // initiator
            StaticKeys(
                std::make_shared<const PublicKey>(kp_initiator.public_key),
                std::make_shared<const PrivateKey>(kp_initiator.private_key)
            ),
            // responder
            StaticKeys(
                std::make_shared<const PublicKey>(kp_responder.public_key),
                std::make_shared<const PrivateKey>(kp_responder.private_key)
            ),
        };
    }

    void IntegrationFixture::wire()
    {
        // wire the lower layers together
        initiator_lower.configure(stacks.initiator->get_upper(), responder_lower);
        responder_lower.configure(stacks.responder->get_upper(), initiator_lower);

        // wire the upper layers
        initiator_upper.configure(stacks.initiator->get_lower());
        responder_upper.configure(stacks.responder->get_lower());

        // wire the initiator and responder
        stacks.initiator->bind(initiator_lower, initiator_upper);
        stacks.responder->bind(responder_lower, responder_upper);

        // wire the upper layer validators
        initiator_upper.add_validator(initiator_validator);
        responder_upper.add_validator(responder_validator);
    }

}



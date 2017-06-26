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
        responder_lower(exe)
    {
        // we need to first perform some key derivation so we can inject private keys, and share public keys
        KeyPair kp_responder;
        KeyPair kp_initiator;

        Crypto::gen_keypair_x25519(kp_responder);
        Crypto::gen_keypair_x25519(kp_initiator);

        // make copies of the all the keys on the heap
        const auto responder_pub = std::make_shared<const PublicKey>(kp_responder.public_key);
        const auto initiator_pub = std::make_shared<const PublicKey>(kp_initiator.public_key);
        const auto responder_priv = std::make_shared<const PrivateKey>(kp_responder.private_key);
        const auto initiator_priv = std::make_shared<const PrivateKey>(kp_initiator.private_key);

        initiator = Factory::initiator(
                        Addresses(1, 10),
                        InitiatorConfig(),
                        ilog.logger,
                        exe,
                        LocalKeys(initiator_pub, initiator_priv),
                        ICertificateHandler::preshared_key(responder_pub)
                    );

        responder = Factory::responder(
                        Addresses(10, 1),
                        ResponderConfig(),
                        rlog.logger,
                        exe,
                        LocalKeys(responder_pub, responder_priv),
                        ICertificateHandler::preshared_key(initiator_pub)
                    );

        // wire the lower layers together
        initiator_lower.configure(initiator->get_upper(), responder_lower);
        responder_lower.configure(responder->get_upper(), initiator_lower);

        // wire the upper layers
        initiator_upper.configure(initiator->get_lower());
        responder_upper.configure(responder->get_lower());

        // wire the initiator and responder
        initiator->bind(initiator_lower, initiator_upper);
        responder->bind(responder_lower, responder_upper);

        // wire the upper layer validators
        initiator_upper.add_validator(initiator_validator);
        responder_upper.add_validator(responder_validator);
    }

}



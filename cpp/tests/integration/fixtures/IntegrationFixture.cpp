#include "IntegrationFixture.h"

#include "ssp21/crypto/Crypto.h"
#include "ssp21/stack/Factory.h"

#include "ssp21/crypto/gen/CertificateBody.h"
#include "ssp21/crypto/gen/CertificateEnvelope.h"
#include "ssp21/crypto/gen/CertificateChain.h"
#include "ssp21/crypto/gen/ContainerFile.h"

#include "ssp21/util/SerializationUtils.h"
#include "ssp21/util/Exception.h"

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
                                   ICertificateHandler::certificates(nullptr, nullptr)
                               );

        const auto responder = Factory::responder(
                                   Addresses(10, 1),
                                   ResponderConfig(),
                                   ilogger,
                                   exe,
                                   keys.responder,
                                   ICertificateHandler::certificates(nullptr, nullptr)
                               );

        return Stacks{ initiator, responder };
    }

    IntegrationFixture::EndpointKeys IntegrationFixture::generate_random_keys()
    {
        // we need to first perform some key derivation
        KeyPair kp_responder;
        KeyPair kp_initiator;

        Crypto::gen_keypair_x25519(kp_responder);
        Crypto::gen_keypair_x25519(kp_initiator);

        // make copies of the all the keys on the heap

        return EndpointKeys
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

	IntegrationFixture::AuthorityData IntegrationFixture::generate_authority_data()
	{		
		KeyPair kp_authority;
		Crypto::gen_keypair_ed25519(kp_authority);
		
		// now generate a self-signed authority certificate
		const CertificateBody body(
			0x00000000,
			0xFFFFFFFF,
			1,
			PublicKeyType::Ed25519,
			kp_authority.public_key.as_seq()
		);

		const auto body_data = serialize::to_buffer(body);

		DSAOutput signature;
		std::error_code ec;
		Crypto::sign_ed25519(body_data->as_rslice(), kp_authority.private_key.as_seq(), signature, ec);
		if (ec)
		{
			throw Exception("Error signing authority certificate: ", ec.message());
		}
		
		CertificateChain chain;
		chain.certificates.push(
			CertificateEnvelope(
				signature.as_seq(),
				body_data->as_rslice()
			)
		);
		const auto chain_data = serialize::to_buffer(chain);

		return AuthorityData {
			std::make_shared<PrivateKey>(kp_authority.private_key),
			std::make_shared<PublicKey>(kp_authority.public_key),
			serialize::to_secure_buffer(ContainerFile(ContainerEntryType::certificate_chain, chain_data->as_rslice()))
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



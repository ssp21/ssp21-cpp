#include "IntegrationFixture.h"

#include "ssp21/crypto/Crypto.h"
#include "ssp21/stack/Factory.h"

#include "ssp21/crypto/gen/CertificateBody.h"
#include "ssp21/crypto/gen/CertificateEnvelope.h"
#include "ssp21/crypto/gen/CertificateChain.h"
#include "ssp21/crypto/gen/ContainerFile.h"

#include "ssp21/util/SerializationUtils.h"
#include "ssp21/util/Exception.h"
#include "ssp21/util/ConsolePrettyPrinter.h"

#include "openpal/logging/HexLogging.h"

#include "MockKeyStore.h"

namespace ssp21
{

    IntegrationFixture::IntegrationFixture(Mode mode) :
        exe(std::make_shared<exe4cpp::MockExecutor>()),
        ilog(std::make_shared<ConsolePrettyPrinter>(), openpal::ModuleId(0), "initiator", openpal::LogLevels::everything()),
        rlog(std::make_shared<ConsolePrettyPrinter>(), openpal::ModuleId(0), "responder", openpal::LogLevels::everything()),
        initiator_lower(exe),
        responder_lower(exe),
        stacks(this->get_stacks(mode, rlog, ilog, exe))
    {
        this->wire();
    }

    IntegrationFixture::Stacks IntegrationFixture::get_stacks(Mode mode, openpal::Logger rlogger, openpal::Logger ilogger, std::shared_ptr<exe4cpp::IExecutor> exe)
    {
        switch (mode)
        {
        case(Mode::preshared_key):
            return preshared_key_stacks(rlogger, ilogger, exe);
        case(Mode::certificates):
            return certificate_stacks(rlogger, ilogger, exe);
        case(Mode::shared_secret):
            return shared_secret_stacks(rlogger, ilogger, exe);
        case(Mode::qkd):
            return qkd_stacks(rlogger, ilogger, exe);
        default:
            throw new Exception("Unsupported integration test mode");
        }

    }

    IntegrationFixture::Stacks IntegrationFixture::preshared_key_stacks(openpal::Logger rlogger, openpal::Logger ilogger, std::shared_ptr<exe4cpp::IExecutor> exe)
    {
        const auto keys = generate_random_keys();

        const auto initiator = initiator::factory::preshared_public_key_mode(
                                   Addresses(1, 10),
                                   InitiatorConfig(),
                                   ilogger,
                                   exe,
                                   CryptoSuite(), // defaults
                                   keys.initiator,
                                   keys.responder.public_key
                               );

        const auto responder = responder::factory::preshared_public_key_mode(
                                   Addresses(10, 1),
                                   ResponderConfig(),
                                   rlogger,
                                   exe,
                                   keys.responder,
                                   keys.initiator.public_key
                               );

        return Stacks{ initiator, responder };
    }

    IntegrationFixture::Stacks IntegrationFixture::qkd_stacks(openpal::Logger rlogger, openpal::Logger ilogger, std::shared_ptr<exe4cpp::IExecutor> exe)
    {
        const auto key_store = std::make_shared<MockKeyStore>();

        CryptoSuite suite;
        suite.handshake_ephemeral = HandshakeEphemeral::none;

        const auto initiator = initiator::factory::qkd_mode(
                                   Addresses(1, 10),
                                   InitiatorConfig(),
                                   ilogger,
                                   exe,
                                   suite,
                                   key_store
                               );

        const auto responder = responder::factory::qkd_mode(
                                   Addresses(10, 1),
                                   ResponderConfig(),
                                   rlogger,
                                   exe,
                                   key_store
                               );

        return Stacks{ initiator, responder };
    }

    IntegrationFixture::Stacks IntegrationFixture::certificate_stacks(openpal::Logger rlogger, openpal::Logger ilogger, std::shared_ptr<exe4cpp::IExecutor> exe)
    {
        const auto keys = generate_random_keys();

        //
        const auto authority_data = generate_authority_data();

        // produce certifivate data for each endpoint, signing it with the authority private key
        const auto initiator_cert_data = make_cert_file_data(*keys.initiator.public_key, PublicKeyType::X25519, 0, *authority_data.private_key);
        const auto responder_cert_data = make_cert_file_data(*keys.responder.public_key, PublicKeyType::X25519, 0, *authority_data.private_key);

        const auto initiator = initiator::factory::certificate_public_key_mode(
                                   Addresses(1, 10),
                                   InitiatorConfig(),
                                   ilogger,
                                   exe,
                                   CryptoSuite(), // defaults
                                   keys.initiator,
                                   authority_data.certificate_file_data,
                                   initiator_cert_data
                               );

        const auto responder = responder::factory::certificate_public_key_mode(
                                   Addresses(10, 1),
                                   ResponderConfig(),
                                   rlogger,
                                   exe,
                                   keys.responder,
                                   authority_data.certificate_file_data,
                                   responder_cert_data
                               );

        return Stacks{ initiator, responder };
    }

    IntegrationFixture::Stacks IntegrationFixture::shared_secret_stacks(openpal::Logger rlogger, openpal::Logger ilogger, std::shared_ptr<exe4cpp::IExecutor> exe)
    {
        const auto shared_secret = generate_shared_secret();

        CryptoSuite suite;
        suite.handshake_ephemeral = HandshakeEphemeral::nonce;

        const auto initiator = initiator::factory::shared_secert_mode(
                                   Addresses(1, 10),
                                   InitiatorConfig(),
                                   ilogger,
                                   exe,
                                   suite,
                                   shared_secret
                               );

        const auto responder = responder::factory::shared_secret_mode(
                                   Addresses(10, 1),
                                   ResponderConfig(),
                                   rlogger,
                                   exe,
                                   shared_secret
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

    std::shared_ptr<const SymmetricKey> IntegrationFixture::generate_shared_secret()
    {
        const std::shared_ptr<SymmetricKey> key = std::make_shared<SymmetricKey>();

        Crypto::gen_random(key->as_wseq().take(consts::crypto::symmetric_key_length));
        key->set_type(BufferType::symmetric_key);

        return key;
    }

    IntegrationFixture::AuthorityData IntegrationFixture::generate_authority_data()
    {
        KeyPair kp_authority;
        Crypto::gen_keypair_ed25519(kp_authority);
        const auto cert_file_data = make_cert_file_data(kp_authority.public_key, PublicKeyType::Ed25519, 1, kp_authority.private_key);

        return AuthorityData
        {
            std::make_shared<PrivateKey>(kp_authority.private_key),
            std::make_shared<PublicKey>(kp_authority.public_key),
            cert_file_data
        };
    }

    std::shared_ptr<SecureDynamicBuffer> IntegrationFixture::make_cert_file_data(const PublicKey& public_key, PublicKeyType public_key_type, uint8_t signing_level, const PrivateKey& signing_key)
    {
        const auto body_data = serialize::to_buffer(
                                   CertificateBody(
                                       0x00000000,
                                       0xFFFFFFFF,
                                       signing_level,
                                       public_key_type,
                                       public_key.as_seq()
                                   )
                               );

        DSAOutput signature;
        std::error_code ec;
        Crypto::sign_ed25519(body_data->as_rslice(), signing_key.as_seq(), signature, ec);
        if (ec)
        {
            throw Exception("Error signing certificate: ", ec.message());
        }

        CertificateChain chain;
        chain.certificates.push(
            CertificateEnvelope(
                signature.as_seq(),
                body_data->as_rslice()
            )
        );

        const auto chain_data = serialize::to_buffer(chain);

        const ContainerFile container(ContainerEntryType::certificate_chain, chain_data->as_rslice());

        return serialize::to_secure_buffer(container);
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



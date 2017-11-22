#ifndef SSP21_INTEGRATION_FIXTURE_H
#define SSP21_INTEGRATION_FIXTURE_H

#include "../mocks/LowerLayer.h"
#include "../mocks/UpperLayer.h"
#include "../mocks/SeqValidator.h"

#include "testlib/MockLogHandler.h"
#include "ssp21/util/ConsolePrettyPrinter.h"

#include "ssp21/stack/IStack.h"
#include "ssp21/crypto/StaticKeys.h"
#include "ssp21/crypto/gen/PublicKeyType.h"
#include "ssp21/util/SecureDynamicBuffer.h"

namespace ssp21
{
    enum class Mode : uint8_t
    {
        preshared_key,
        certificates,
        shared_secret,
        qkd
    };



    class IntegrationFixture
    {
        struct Stacks
        {
            const std::shared_ptr<IStack> initiator;
            const std::shared_ptr<IStack> responder;
        };

        struct EndpointKeys
        {
            StaticKeys initiator;
            StaticKeys responder;
        };

        struct AuthorityData
        {
            const std::shared_ptr<PrivateKey> private_key;
            const std::shared_ptr<PublicKey> public_key;
            const std::shared_ptr<SecureDynamicBuffer> certificate_file_data;
        };

    public:

        IntegrationFixture(Mode mode);

        void log_everything_to_stdio()
        {
            this->ilog.log_everything();
            this->ilog.add_backend(std::make_shared<ConsolePrettyPrinter>());

            this->rlog.log_everything();
            this->rlog.add_backend(std::make_shared<ConsolePrettyPrinter>());
        }

        const std::shared_ptr<openpal::MockExecutor> exe;
        openpal::MockLogHandler ilog;
        openpal::MockLogHandler rlog;

        LowerLayer initiator_lower;
        LowerLayer responder_lower;

        UpperLayer initiator_upper;
        UpperLayer responder_upper;

        const std::shared_ptr<SeqValidator> initiator_validator = SeqValidator::create();
        const std::shared_ptr<SeqValidator> responder_validator = SeqValidator::create();

        Stacks stacks;

    private:

        static Stacks get_stacks(Mode mode, openpal::Logger rlogger, openpal::Logger ilogger, std::shared_ptr<openpal::IExecutor> exe);

        static Stacks preshared_key_stacks(openpal::Logger rlogger, openpal::Logger ilogger, std::shared_ptr<openpal::IExecutor> exe);

        static Stacks qkd_stacks(openpal::Logger rlogger, openpal::Logger ilogger, std::shared_ptr<openpal::IExecutor> exe);

        static Stacks certificate_stacks(openpal::Logger rlogger, openpal::Logger ilogger, std::shared_ptr<openpal::IExecutor> exe);

        static Stacks shared_secret_stacks(openpal::Logger rlogger, openpal::Logger ilogger, std::shared_ptr<openpal::IExecutor> exe);

        static EndpointKeys generate_random_keys();

        static std::shared_ptr<const SymmetricKey> generate_shared_secret();

        static AuthorityData generate_authority_data();

        static std::shared_ptr<SecureDynamicBuffer> make_cert_file_data(const PublicKey& public_key, PublicKeyType public_key_type, uint8_t signing_level, const PrivateKey& signing_key);

        void wire();
    };

}

#endif

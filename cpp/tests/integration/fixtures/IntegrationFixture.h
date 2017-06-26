#ifndef SSP21_INTEGRATION_FIXTURE_H
#define SSP21_INTEGRATION_FIXTURE_H

#include "../mocks/LowerLayer.h"
#include "../mocks/UpperLayer.h"
#include "../mocks/SeqValidator.h"

#include "testlib/MockLogHandler.h"

#include "ssp21/stack/IStack.h"
#include "ssp21/crypto/StaticKeys.h"

namespace ssp21
{

    class IntegrationFixture
    {
        struct Stacks
        {
            const std::shared_ptr<IStack> initiator;
            const std::shared_ptr<IStack> responder;
        };

        struct Keys
        {
            StaticKeys initiator;
            StaticKeys responder;
        };

    public:

        IntegrationFixture();

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

        static Stacks preshared_key_stacks(openpal::Logger rlogger, openpal::Logger ilogger, std::shared_ptr<openpal::IExecutor> exe);

        static Stacks certificate_stacks(openpal::Logger rlogger, openpal::Logger ilogger, std::shared_ptr<openpal::IExecutor> exe);


        static Keys generate_random_keys();

        void wire();
    };

}

#endif

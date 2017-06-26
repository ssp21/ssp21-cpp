#ifndef SSP21_INTEGRATION_FIXTURE_H
#define SSP21_INTEGRATION_FIXTURE_H

#include "../mocks/LowerLayer.h"
#include "../mocks/UpperLayer.h"
#include "../mocks/SeqValidator.h"

#include "testlib/MockLogHandler.h"

#include "ssp21/stack/IStack.h"

namespace ssp21
{

    class IntegrationFixture
    {

    public:

        IntegrationFixture();

        const std::shared_ptr<openpal::MockExecutor> exe;
        openpal::MockLogHandler ilog;
        openpal::MockLogHandler rlog;

        LowerLayer initiator_lower;
        LowerLayer responder_lower;

        UpperLayer initiator_upper;
        UpperLayer responder_upper;

        std::shared_ptr<IStack> initiator;
        std::shared_ptr<IStack> responder;

        const std::shared_ptr<SeqValidator> initiator_validator = SeqValidator::create();
        const std::shared_ptr<SeqValidator> responder_validator = SeqValidator::create();
    };

}

#endif

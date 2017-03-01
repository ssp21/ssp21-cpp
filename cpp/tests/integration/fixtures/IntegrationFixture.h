#ifndef SSP21_INTEGRATION_FIXTURE_H
#define SSP21_INTEGRATION_FIXTURE_H

#include "../mocks/LowerLayer.h"
#include "../mocks/UpperLayer.h"

#include "testlib/MockExecutor.h"
#include "testlib/MockLogHandler.h"

#include "ssp21/crypto/Initiator.h"
#include "ssp21/crypto/Responder.h"
#include "ssp21/crypto/MessageOnlyFrameWriter.h"

#include <stdexcept>
#include <deque>

namespace ssp21
{
    class SeqValidator final : public IReceiveValidator
    {
    public:

        virtual void validate(const seq32_t& data) override
        {
            if (this->expected_seqs.empty())
            {
                throw std::logic_error("not expecting data!");
            }

            const auto expected = expected_seqs.front();
            expected_seqs.pop_front();

            if (!data.equals(expected))
            {
                throw std::logic_error("bad comparison!");
            }
        }

        void expect(const seq32_t& data)
        {
            this->expected_seqs.push_back(data);
        }

        bool is_empty() const
        {
            return expected_seqs.empty();
        }

        static std::shared_ptr<SeqValidator> create()
        {
            return std::make_shared<SeqValidator>();
        }

    private:

        std::deque<seq32_t> expected_seqs;
    };

    class IntegrationFixture
    {

    public:

        IntegrationFixture() :
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

            initiator = std::make_unique<Initiator>(
                            InitiatorConfig(),
                            Session::Config(),
                            ilog.logger,
                            std::make_shared<MessageOnlyFrameWriter>(ilog.logger),
                            exe,
                            Keys(initiator_pub, responder_pub, initiator_priv)
                        );

            responder = std::make_unique<Responder>(
                            ResponderConfig(),
                            Session::Config(),
                            rlog.logger,
                            std::make_shared<MessageOnlyFrameWriter>(rlog.logger),
                            exe,
                            Keys(responder_pub, initiator_pub, responder_priv)
                        );

            // wire the lower layers together
            initiator_lower.configure(*initiator, responder_lower);
            responder_lower.configure(*responder, initiator_lower);

            // wire the upper layers
            initiator_upper.configure(*initiator);
            responder_upper.configure(*responder);

            // wire the initiator and responder
            initiator->bind(initiator_lower, initiator_upper);
            responder->bind(responder_lower, responder_upper);

            // wire the upper layer validators
            initiator_upper.add_validator(initiator_validator);
            responder_upper.add_validator(responder_validator);
        }


        const std::shared_ptr<openpal::MockExecutor> exe;
        openpal::MockLogHandler ilog;
        openpal::MockLogHandler rlog;

        LowerLayer initiator_lower;
        LowerLayer responder_lower;

        UpperLayer initiator_upper;
        UpperLayer responder_upper;

        std::unique_ptr<Initiator> initiator;
        std::unique_ptr<Responder> responder;

        const std::shared_ptr<SeqValidator> initiator_validator = SeqValidator::create();
        const std::shared_ptr<SeqValidator> responder_validator = SeqValidator::create();
    };



}

#endif

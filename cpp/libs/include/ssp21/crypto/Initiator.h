
#ifndef SSP21_INITIATOR_H
#define SSP21_INITIATOR_H

#include "ssp21/crypto/CryptoLayer.h"

namespace ssp21
{
    /**
    	Initiator implementation - Inherits most of its functionality from the CryptoLayer base class.

    	Adds initiator hand shaking support and renegotiation support
    */
    class Initiator final : public CryptoLayer
    {

    public:

        struct Config : public CryptoLayer::Config {};

        Initiator(
            const Config& context_config,
            const Session::Config& session_config,
            const openpal::Logger& logger,
            const std::shared_ptr<IFrameWriter>& frame_writer,
            const std::shared_ptr<openpal::IExecutor>& executor,
            std::unique_ptr<KeyPair> local_static_key_pair,
            std::unique_ptr<PublicKey> remote_static_public_key
        );

        struct IHandshakeState
        {
            virtual IHandshakeState& on_message(Initiator& ctx, const seq32_t& msg_bytes, const ReplyHandshakeBegin& msg) = 0;
            virtual IHandshakeState& on_message(Initiator& ctx, const seq32_t& msg_bytes, const ReplyHandshakeAuth& msg) = 0;
            virtual IHandshakeState& on_message(Initiator& ctx, const seq32_t& msg_bytes, const ReplyHandshakeError& msg) = 0;
        };

    private:

        // ---- private helper methods -----


        // ---- implement CryptoLayer -----

        virtual void reset_state_on_close() override;

        virtual bool supports(Function function) const override;

        virtual void on_parse_error(Function function, ParseError error) override {}

        virtual bool on_message(const ReplyHandshakeBegin& msg, const seq32_t& raw_data, const openpal::Timestamp& now) override;

        virtual bool on_message(const ReplyHandshakeAuth& msg, const seq32_t& raw_data, const openpal::Timestamp& now) override;

        virtual bool on_message(const ReplyHandshakeError& msg, const seq32_t& raw_data, const openpal::Timestamp& now) override;

        // ---- private members -----

    };

}

#endif

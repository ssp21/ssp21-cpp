
#ifndef SSP21_RESPONDER_H
#define SSP21_RESPONDER_H

#include "ssp21/crypto/gen/RequestHandshakeBegin.h"
#include "ssp21/crypto/gen/RequestHandshakeAuth.h"
#include "ssp21/crypto/gen/SessionData.h"

#include "ssp21/crypto/gen/Function.h"
#include "ssp21/crypto/gen/HandshakeError.h"

#include "ssp21/link/LinkConstants.h"
#include "ssp21/LayerInterfaces.h"

#include "ssp21/crypto/Handshake.h"
#include "ssp21/crypto/Session.h"
#include "ssp21/crypto/Reassembler.h"
#include "ssp21/crypto/TxState.h"
#include "ssp21/crypto/IMessageHandler.h"

#include "ssp21/IFrameWriter.h"
#include "ssp21/LogLevels.h"

#include "openpal/logging/Logger.h"
#include "openpal/container/Buffer.h"
#include "openpal/executor/IExecutor.h"

#include <memory>


namespace ssp21
{
    /**
    	WIP - this class will implement the stateful part of the responder.
    */
    class Responder final : public IUpperLayer, public ILowerLayer, private IMessageHandler
    {

    public:

        struct Config
        {
            // configuration for the session
            Session::Config session;

            /// The maximum size of a reassembled message
            uint16_t max_reassembly_size = consts::link::max_config_payload_size;
        };

        struct Context
        {
            Context(
                const Config& config,
                std::unique_ptr<IFrameWriter> frame_writer,
                std::unique_ptr<KeyPair> local_static_key_pair,
                std::unique_ptr<PublicKey> remote_static_public_key,
                const openpal::Logger& logger,
                const std::shared_ptr<openpal::IExecutor>& executor,
                ILowerLayer& lower
            );

            void log_message(openpal::LogLevel msg_level, openpal::LogLevel field_level, Function func, const IMessage& msg);

            template <class T>
            FormatResult transmit_to_lower(const T& msg);

            void reply_with_handshake_error(HandshakeError err);

            void set_upper_layer(IUpperLayer& upper)
            {
                this->upper = &upper;
            }

            HandshakeError validate(const RequestHandshakeBegin& msg);

            Config config;
            const std::unique_ptr<IFrameWriter> frame_writer;

            std::unique_ptr<KeyPair> local_static_key_pair;
            std::unique_ptr<PublicKey> remote_static_public_key;

            openpal::Logger logger;

            const std::shared_ptr<openpal::IExecutor> executor;

            Handshake handshake;
            Session session;
            Reassembler reassembler;
            TxState tx_state;

            ILowerLayer* const lower;
            IUpperLayer* upper = nullptr;
        };

        struct IHandshakeState
        {
            virtual IHandshakeState& on_message(Context& ctx, const seq32_t& msg_bytes, const RequestHandshakeBegin& msg) = 0;
            virtual IHandshakeState& on_message(Context& ctx, const seq32_t& msg_bytes, const RequestHandshakeAuth& msg) = 0;
        };

        Responder(const Config& config,
                  std::unique_ptr<IFrameWriter> frame_writer,
                  std::unique_ptr<KeyPair> local_static_key_pair,
                  std::unique_ptr<PublicKey> remote_static_public_key,
                  const openpal::Logger& logger,
                  const std::shared_ptr<openpal::IExecutor>& executor,
                  ILowerLayer& lower
                 );

        void set_upper_layer(IUpperLayer& upper)
        {
            this->ctx.set_upper_layer(upper);
        }

        ResponderStatistics get_statistics() const
        {
            return ResponderStatistics(this->ctx.session.get_statistics());
        }

        // ---- implement ILowerLayer -----

        virtual bool transmit(const seq32_t& data) override;
        virtual void receive() override;

    private:

        void check_receive();
        void check_transmit();

        inline bool can_receive() const
        {
            return ctx.lower->get_is_tx_ready() && !this->is_rx_ready;
        }

        void process(const seq32_t& data);

        // ---- implement IUpperLayer -----

        virtual void on_open_impl() override {}
        virtual void on_close_impl() override;
        virtual void on_tx_ready_impl() override;
        virtual bool on_rx_ready_impl(const seq32_t& data) override;

        // ---- implement IMessageHandler -----

        virtual bool supports(Function function) const override;

        virtual void on_parse_error(Function function, ParseError error) override;

        virtual bool on_message(const RequestHandshakeBegin& msg, const seq32_t& raw_data, const openpal::Timestamp& now) override;

        virtual bool on_message(const RequestHandshakeAuth& msg, const seq32_t& raw_data, const openpal::Timestamp& now) override;

        virtual bool on_message(const SessionData& msg, const seq32_t& raw_data, const openpal::Timestamp& now) override;

        // ---- private members -----

        // All of the state in the responder except for the actual state instances
        Context ctx;

        // state instance for the handshake
        IHandshakeState* handshake_state;
    };

    template <class T>
    FormatResult Responder::Context::transmit_to_lower(const T& msg)
    {
        this->log_message(levels::tx_crypto_msg, levels::tx_crypto_msg_fields, T::function, msg);

        const auto res = this->frame_writer->write(msg);
        if (res.is_error()) return res;

        this->lower->transmit(res.frame);

        return res;
    }

}

#endif

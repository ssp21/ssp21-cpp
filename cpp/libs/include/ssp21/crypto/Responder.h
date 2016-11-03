
#ifndef SSP21_RESPONDER_H
#define SSP21_RESPONDER_H

#include "ssp21/crypto/gen/RequestHandshakeBegin.h"
#include "ssp21/crypto/gen/RequestHandshakeAuth.h"
#include "ssp21/crypto/gen/UnconfirmedSessionData.h"

#include "ssp21/crypto/gen/Function.h"
#include "ssp21/crypto/gen/HandshakeError.h"

#include "ssp21/link/LinkConstants.h"
#include "ssp21/LayerInterfaces.h"

#include "ssp21/crypto/Handshake.h"
#include "ssp21/crypto/Session.h"

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
    class Responder final : public IUpperLayer, public ILowerLayer, private IMessageProcessor
    {

    public:

        struct Config
        {
            /// The maximum message size that this layer should transmit to the link layer
            /// This constant determines the size of a buffer allocated when the responder
            /// is constructed
            uint16_t max_tx_message_size = consts::link::max_config_payload_size;

            /// expected remote address
            uint16_t remote_address = consts::link::default_responder_remote_address;

            /// local address
            uint16_t local_address = consts::link::default_responder_local_address;
        };

        struct Context
        {
            Context(
                const Config& config,
                std::unique_ptr<KeyPair> local_static_key_pair,
                std::unique_ptr<PublicKey> remote_static_public_key,
                const openpal::Logger& logger,
                const std::shared_ptr<openpal::IExecutor>& executor,
                ILowerLayer& lower
            );

            void log_message(openpal::LogLevel msg_level, openpal::LogLevel field_level, Function func, const IMessage& msg, uint32_t length);

            template <class T>
            void transmit_to_lower(const T& msg, const openpal::RSlice& data)
            {
                this->log_message(levels::tx_crypto_msg, levels::tx_crypto_msg_fields, T::function, msg, data.length());

                this->lower->transmit(
                    Message(
                        Addresses(config.remote_address, config.local_address),
                        data
                    )
                );
            }

            void reply_with_handshake_error(HandshakeError err);

            void set_upper_layer(IUpperLayer& upper)
            {
                this->upper = &upper;
            }

            FormatResult write_msg(const IMessage& msg)
            {
                auto dest = tx_buffer.as_wslice();
                return msg.write(dest);
            }

            HandshakeError validate(const RequestHandshakeBegin& msg);

            Config config;

            std::unique_ptr<KeyPair> local_static_key_pair;
            std::unique_ptr<PublicKey> remote_static_public_key;

            openpal::Logger logger;

            const std::shared_ptr<openpal::IExecutor> executor;

            Handshake handshake;
            Session session;

            ILowerLayer* const lower;
            IUpperLayer* upper = nullptr;

        private:

            openpal::Buffer tx_buffer;
        };

        struct IHandshakeState
        {
            virtual IHandshakeState& on_message(Context& ctx, const openpal::RSlice& msg_bytes, const RequestHandshakeBegin& msg) = 0;
            virtual IHandshakeState& on_message(Context& ctx, const openpal::RSlice& msg_bytes, const RequestHandshakeAuth& msg) = 0;
        };

        Responder(const Config& config,
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

    private:

        bool can_receive() const
        {
            return ctx.lower->is_tx_ready() && !this->is_rx_ready();
        }

        // ---- implement IUpperLayer -----

        virtual void on_open_impl() override;
        virtual void on_close_impl() override;
        virtual void on_tx_ready_impl() override;
        virtual void on_rx_ready_impl() override;

        // ---- implement ILowerLayer -----

        virtual bool transmit(const Message& message) override;
        virtual bool receive(IMessageProcessor& processor) override;

        // ---- implement IMessageProcessor -----

        virtual void process(const Message& message) override;

        // ---- private methods -----

        template <class MsgType>
        inline void handle_handshake_message(const openpal::RSlice& data);

        void handle_session_message(const openpal::RSlice& data);

        // ---- private members -----

        // All of the state in the responder except for the actual state instances
        Context ctx;

        // state instance for the handshake
        IHandshakeState* handshake_state;
    };

}

#endif

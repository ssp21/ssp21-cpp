
#ifndef SSP21_RESPONDER_H
#define SSP21_RESPONDER_H

#include "ssp21/msg/RequestHandshakeBegin.h"
#include "ssp21/msg/RequestHandshakeAuth.h"
#include "ssp21/msg/UnconfirmedSessionData.h"

#include "ssp21/gen/Function.h"
#include "ssp21/gen/HandshakeError.h"

#include "ssp21/link/LinkConstants.h"
#include "ssp21/LayerInterfaces.h"
#include "ssp21/crypto/Handshake.h"

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
                openpal::Logger logger,
                openpal::IExecutor& executor,
                ILowerLayer& lower
            );

            void transmit_to_lower(const openpal::RSlice& msg);

            void reply_with_handshake_error(HandshakeError err);

            void set_upper_layer(IUpperLayer& upper)
            {
                this->upper = &upper;
            }

            template <class T>
            FormatResult write_msg(const T& msg)
            {
                auto dest = tx_buffer.as_wslice();
                return msg.write_msg(dest);
            }

            HandshakeError validate(const RequestHandshakeBegin& msg);

            Config config;

            std::unique_ptr<KeyPair> local_static_key_pair;
            std::unique_ptr<PublicKey> remote_static_public_key;

            openpal::Logger logger;

            openpal::IExecutor* const executor;

            Handshake handshake;
            openpal::Timestamp session_init_time = openpal::Timestamp::min_value();


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
                  openpal::Logger logger,
                  openpal::IExecutor& executor,
                  ILowerLayer& lower
                 );

        void set_upper_layer(IUpperLayer& upper)
        {
            this->ctx.set_upper_layer(upper);
        }

    private:

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

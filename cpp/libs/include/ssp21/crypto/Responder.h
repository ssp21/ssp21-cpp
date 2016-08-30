
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
    class Responder final : public IUpperLayer, private IMessageProcessor
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

			Config config;

			std::unique_ptr<KeyPair> local_static_key_pair;
			std::unique_ptr<PublicKey> remote_static_public_key;

			openpal::Logger logger;
			
			openpal::IExecutor* const executor;						
			ILowerLayer* const lower;

			openpal::Buffer tx_buffer;
			Handshake handshake;
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

    private:

        // ---- implement IUpperLayer -----

        virtual void on_open_impl() override;
        virtual void on_close_impl() override;
        virtual void on_tx_ready_impl() override;
        virtual void on_rx_ready_impl() override;

        // ---- implement IMessageProcessor -----

        virtual void process(const Message& message) override;

        template <class MsgType>
        inline void read_any(const openpal::RSlice& data);

        void on_message(const openpal::RSlice& data, const RequestHandshakeBegin& msg);
        void on_message(const openpal::RSlice& data, const UnconfirmedSessionData& msg);
        void on_message(const openpal::RSlice& data, const RequestHandshakeAuth& msg);

        template <class MsgType>
        void handle_parse_error(ParseError err);

        void reply_with_handshake_error(HandshakeError err);

		HandshakeError validate_handshake_begin(const RequestHandshakeBegin& msg);		

		Context ctx;
		
		// state instance for the handshake
		IHandshakeState* handshake_state;
    };

    template <>
    inline void Responder::handle_parse_error<RequestHandshakeBegin>(ParseError err)
    {
        this->reply_with_handshake_error(HandshakeError::bad_message_format);
    }

    template <>
    inline void Responder::handle_parse_error<RequestHandshakeAuth>(ParseError err)
    {
        this->reply_with_handshake_error(HandshakeError::bad_message_format);
    }

    template <>
    inline void Responder::handle_parse_error<UnconfirmedSessionData>(ParseError err) {}
}

#endif

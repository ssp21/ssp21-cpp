
#ifndef SSP21_RESPONDER_H
#define SSP21_RESPONDER_H

#include "ssp21/crypto/CryptoLayer.h"

#include "ssp21/LogLevels.h"

#include "openpal/container/Buffer.h"


#include <memory>


namespace ssp21
{
    /**
    	Responder implementation. Is an upper and lower layer. Dispatches messages
    	to itself via private IMessageHandler.
    */
    class Responder final : public CryptoLayer
    {
		friend class HandshakeIdle;
		friend class HandshakeWaitForAuth;

    public:

        struct Config : public CryptoLayer::Config {};       

        struct IHandshakeState
        {
            virtual IHandshakeState& on_message(Responder& ctx, const seq32_t& msg_bytes, const RequestHandshakeBegin& msg) = 0;
            virtual IHandshakeState& on_message(Responder& ctx, const seq32_t& msg_bytes, const RequestHandshakeAuth& msg) = 0;
        };

        Responder(
            const Config& context_config,
            const Session::Config& session_config,
            const openpal::Logger& logger,
            const std::shared_ptr<IFrameWriter>& frame_writer,
            const std::shared_ptr<openpal::IExecutor>& executor,
            std::unique_ptr<KeyPair> local_static_key_pair,
            std::unique_ptr<PublicKey> remote_static_public_key
        );
                
        ResponderStatistics get_statistics() const
        {
            return ResponderStatistics(this->session.get_statistics());
        }

        // ---- implement ILowerLayer -----

        virtual bool transmit(const seq32_t& data) override;
        
		virtual void receive() override;

    private:

		// ---- private helper methods -----

        void check_receive();

        void check_transmit();
		
		inline bool can_receive() const { return this->lower->get_is_tx_ready() && !this->is_rx_ready; }        
		
		void reply_with_handshake_error(HandshakeError err);
		
		HandshakeError configure_feature_support(const RequestHandshakeBegin& msg);

        // ---- implement IUpperLayer -----

        virtual void on_open_impl() override {}
        
		virtual void on_close_impl() override;
        
		virtual void on_tx_ready_impl() override;
        
		virtual bool on_rx_ready_impl(const seq32_t& data) override;

        // ---- implement CryptoLayer -----

        virtual bool supports(Function function) const override;

        virtual void on_parse_error(Function function, ParseError error) override;

        virtual bool on_message(const RequestHandshakeBegin& msg, const seq32_t& raw_data, const openpal::Timestamp& now) override;

        virtual bool on_message(const RequestHandshakeAuth& msg, const seq32_t& raw_data, const openpal::Timestamp& now) override;

        virtual bool on_message(const SessionData& msg, const seq32_t& raw_data, const openpal::Timestamp& now) override;

        // ---- private members -----        

        // state instance for the handshake
        IHandshakeState* handshake_state;
    };

}

#endif

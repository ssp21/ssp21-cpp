
#ifndef SSP21_SHAREDSECRETINITIATORHANDSHAKE_H
#define SSP21_SHAREDSECRETINITIATORHANDSHAKE_H

#include "ssp21/crypto/IInitiatorHandshake.h"

#include "ssp21/crypto/AlgorithmSet.h"

namespace ssp21
{

    /**
    * Class that holds all of the data used during the handshake process
    */

    class SharedSecretInitiatorHandshake final : public IInitiatorHandshake
    {
    public:

        SharedSecretInitiatorHandshake(const openpal::Logger& logger, const CryptoSuite& crypto_suite, const std::shared_ptr<const SymmetricKey>& shared_secret);

        inline static std::shared_ptr<IInitiatorHandshake> make_shared(const openpal::Logger& logger, const CryptoSuite& crypto_suite, const std::shared_ptr<const SymmetricKey>& shared_secret)
        {
            return std::make_shared<SharedSecretInitiatorHandshake>(logger, crypto_suite, shared_secret);
        }

        virtual seq32_t generate_ephemeral_data() override;

        virtual void begin_request_transmit(const seq32_t& data, const openpal::Timestamp& now) override;

        virtual bool initialize_session(const ReplyHandshakeBegin& msg, const seq32_t& msg_bytes, const InitiatorConfig::Params& params, const openpal::Timestamp& now, Session& session) override;

        virtual HandshakeMode get_handshake_mode() const override
        {
            return HandshakeMode::shared_secret;
        }

        inline seq32_t get_mode_data() const override
        {
            return seq32_t::empty();
        }

        virtual CryptoSuite get_crypto_suite() const override
        {
            return this->crypto_suite;
        }

    private:

        seq32_t mix_handshake_hash(const seq32_t& data);

        openpal::Logger logger;
        const CryptoSuite crypto_suite;
        const shared_secret_algorithms_t algorithms;
        const std::shared_ptr<const SymmetricKey> shared_secret;
        openpal::StaticBuffer<uint32_t, consts::crypto::nonce_length> nonce_buffer;

        // time that the request was transmitted
        openpal::Timestamp time_request_tx;

        // running hash value of first 2 handshake messages (h)
        HashOutput handshake_hash;
    };




}

#endif

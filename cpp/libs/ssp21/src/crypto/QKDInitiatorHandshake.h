
#ifndef SSP21_QKDINITIATORHANDSHAKE_H
#define SSP21_QKDINITIATORHANDSHAKE_H

#include "crypto/IInitiatorHandshake.h"

#include "crypto/AlgorithmSet.h"
#include "ssp21/crypto/IKeySource.h"

namespace ssp21
{

    /**
    * Class that holds all of the data used during the handshake process
    */

    class QKDInitiatorHandshake final : public IInitiatorHandshake
    {
    public:

        QKDInitiatorHandshake(const openpal::Logger& logger, const CryptoSuite& crypto_suite, const std::shared_ptr<IKeySource>& key_source);

        inline static std::shared_ptr<IInitiatorHandshake> make_shared(const openpal::Logger& logger, const CryptoSuite& crypto_suite, const std::shared_ptr<IKeySource>& key_source)
        {
            return std::make_shared<QKDInitiatorHandshake>(logger, crypto_suite, key_source);
        }

        virtual InitResult initialize_new_handshake() override;

        virtual void finalize_request_tx(const seq32_t& request_data, const exe4cpp::steady_time_t& now) override;

        virtual bool initialize_session(const ReplyHandshakeBegin& msg, const seq32_t& msg_bytes, const SessionLimits& limits, const exe4cpp::steady_time_t& now, Session& session) override;

        virtual HandshakeMode get_handshake_mode() const override
        {
            return HandshakeMode::quantum_key_distribution;
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

        // source of keys
        const std::shared_ptr<IKeySource> key_source;

        // the key to use for the current handshake
        std::shared_ptr<const SymmetricKey> key;

        // buffer that stores the key identifier
        ser4cpp::StaticBuffer<uint32_t, ser4cpp::UInt64::size> key_id_buffer;

        // time that the request was transmitted
        exe4cpp::steady_time_t time_request_tx;

        // running hash value of first 2 handshake messages (h)
        HashOutput handshake_hash;
    };




}

#endif

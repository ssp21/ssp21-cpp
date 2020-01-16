
#ifndef SSP21_SHAREDSECRETINITIATORHANDSHAKE_H
#define SSP21_SHAREDSECRETINITIATORHANDSHAKE_H

#include "crypto/IInitiatorHandshake.h"

#include "crypto/AlgorithmSet.h"

namespace ssp21 {

/**
    * Class that holds all of the data used during the handshake process
    */

class SharedSecretInitiatorHandshake final : public IInitiatorHandshake {
public:
    SharedSecretInitiatorHandshake(const log4cpp::Logger& logger, const CryptoSuite& crypto_suite, const std::shared_ptr<const SymmetricKey>& key);

    inline static std::shared_ptr<IInitiatorHandshake> make_shared(const log4cpp::Logger& logger, const CryptoSuite& crypto_suite, const std::shared_ptr<const SymmetricKey>& key)
    {
        return std::make_shared<SharedSecretInitiatorHandshake>(logger, crypto_suite, key);
    }

    virtual InitResult initialize_new_handshake() override;

    virtual void finalize_request_tx(const seq32_t& request_data, const exe4cpp::steady_time_t& now) override;

    virtual bool initialize_session(const ReplyHandshakeBegin& msg, const seq32_t& msg_bytes, const SessionLimits& limits, const exe4cpp::steady_time_t& now, Session& session) override;

    virtual HandshakeMode get_handshake_mode() const override
    {
        return HandshakeMode::shared_secret;
    }

    virtual CryptoSuite get_crypto_suite() const override
    {
        return this->crypto_suite;
    }

private:
    seq32_t mix_handshake_hash(const seq32_t& data);

    log4cpp::Logger logger;
    const CryptoSuite crypto_suite;
    const shared_secret_algorithms_t algorithms;
    const std::shared_ptr<const SymmetricKey> key;

    ser4cpp::StaticBuffer<uint32_t, consts::crypto::nonce_length> nonce_buffer;

    // time that the request was transmitted
    exe4cpp::steady_time_t time_request_tx;

    // running hash value of first 2 handshake messages (h)
    HashOutput handshake_hash;
};

}

#endif

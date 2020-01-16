
#ifndef SSP21_PUBLICKEYINITIATORHANDSHAKE_H
#define SSP21_PUBLICKEYINITIATORHANDSHAKE_H

#include "crypto/IInitiatorHandshake.h"

#include "crypto/AlgorithmSet.h"
#include "ssp21/crypto/ICertificateHandler.h"
#include "ssp21/crypto/StaticKeys.h"

namespace ssp21 {

/**
    * Class that holds all of the data used during the handshake process
    */
class PublicKeyInitiatorHandshake final : public IInitiatorHandshake {
public:
    PublicKeyInitiatorHandshake(const log4cpp::Logger& logger, const StaticKeys& static_keys, const CryptoSuite& crypto_suite, const std::shared_ptr<ICertificateHandler>& cert_handler)
        : logger(logger)
        , static_keys(static_keys)
        , crypto_suite(crypto_suite)
        , algorithms(crypto_suite)
        , cert_handler(cert_handler)
    {
    }

    inline static std::shared_ptr<IInitiatorHandshake> make_shared(const log4cpp::Logger& logger, const StaticKeys& static_keys, const CryptoSuite& crypto_suite, const std::shared_ptr<ICertificateHandler>& cert_handler)
    {
        return std::make_shared<PublicKeyInitiatorHandshake>(logger, static_keys, crypto_suite, cert_handler);
    }

    virtual InitResult initialize_new_handshake() override;

    virtual void finalize_request_tx(const seq32_t& request_data, const exe4cpp::steady_time_t& now) override;

    virtual bool initialize_session(const ReplyHandshakeBegin& msg, const seq32_t& msg_bytes, const SessionLimits& limits, const exe4cpp::steady_time_t& now, Session& session) override;

    virtual HandshakeMode get_handshake_mode() const override
    {
        return cert_handler->mode();
    }

    virtual CryptoSuite get_crypto_suite() const override
    {
        return this->crypto_suite;
    }

private:
    seq32_t mix_handshake_hash(const seq32_t& data);

    log4cpp::Logger logger;

    const StaticKeys static_keys;

    // specific algorithms used to perform steps
    const CryptoSuite crypto_suite;
    const public_key_algorithms_t algorithms;
    const std::shared_ptr<ICertificateHandler> cert_handler;

    // ephemeral keys
    KeyPair local_ephemeral_keys;

    // time that the request was transmitted
    exe4cpp::steady_time_t time_request_tx;

    // running hash value of first 2 handshake messages (h)
    HashOutput handshake_hash;
};

}

#endif

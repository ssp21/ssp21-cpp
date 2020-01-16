
#ifndef SSP21_IINITIATORHANDSHAKE_H
#define SSP21_IINITIATORHANDSHAKE_H

#include "ssp21/util/SequenceTypes.h"

#include "crypto/Session.h"
#include "crypto/gen/ReplyHandshakeBegin.h"
#include "ser4cpp/util/Uncopyable.h"
#include "ssp21/crypto/gen/HandshakeMode.h"

namespace ssp21 {

/**
    * Interface that defines what a concrete initiator handshake needs to do
    */
class IInitiatorHandshake : private ser4cpp::Uncopyable {

public:
    /**
        * Result class returned from initialize_new_handshake()
        */
    class InitResult {
    public:
        // true if we were able to initialize the handshake, false otherwise
        const bool is_success;
        // if succesful, this contains the ephemeral data to send to the responder
        const seq32_t ephemeral_data;
        // if succesful, this contains the mode data to send to the responder
        const seq32_t mode_data;

        static InitResult success(const seq32_t& ephemeral_data, const seq32_t& mode_data)
        {
            return InitResult(true, ephemeral_data, mode_data);
        }

        static InitResult failure()
        {
            return InitResult(false, seq32_t::empty(), seq32_t::empty());
        }

    private:
        InitResult(bool is_success, const seq32_t& ephemeral_data, const seq32_t& mode_data)
            : is_success(is_success)
            , ephemeral_data(ephemeral_data)
            , mode_data(mode_data)
        {
        }
    };

    /**
        * Start the process of initiating a new handshake.
        *
        * 1) Obtain the mode data for handshake
        * 2) Intiailize fresh ephemeral data (DH or nonce)
        *
        * @return HandshakeData instance indicating success/failure and containing ephemeral/mode data fields
        */
    virtual InitResult initialize_new_handshake() = 0;

    /**
        * Initialize the handshake hash and record the time at which the request transmission started
        *
        * @param request_data The seralized data of the RequestHandshakeBegin message used to initialize the handshake hash
        * @param the time request transmission started so that the latency can be measured for the response
        */
    virtual void finalize_request_tx(const seq32_t& request_data, const exe4cpp::steady_time_t& now) = 0;

    /**
        * Process the response to the message, and if successful, initialize the Session
        *
        * @param msg the fully parsed reply message
        * @param response_data the raw bytes corresponding to the parsed message
        * @param limits limits to place on the initialized session
        * @param now the time the message was received
        * @param session the Session to initialize
        * @return True if successful and the session was initialized, false otherwise
        */
    virtual bool initialize_session(const ReplyHandshakeBegin& msg, const seq32_t& response_data, const SessionLimits& limits, const exe4cpp::steady_time_t& now, Session& session) = 0;

    /**
        * @return the handshake mode enumeration that this class implements
        */
    virtual HandshakeMode get_handshake_mode() const = 0;

    /**
        * The crypto suite that this handshake mode is configured to use
        */
    virtual CryptoSuite get_crypto_suite() const = 0;
};

}

#endif

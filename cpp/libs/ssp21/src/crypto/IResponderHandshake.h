
#ifndef SSP21_IRESPONDERHANDSHAKE_H
#define SSP21_IRESPONDERHANDSHAKE_H

#include "ssp21/util/SequenceTypes.h"

#include "ssp21/crypto/gen/HandshakeError.h"
#include "crypto/gen/RequestHandshakeBegin.h"

#include "crypto/Session.h"


namespace ssp21
{

    /**
    * Interface that defines what a concrete responder handshake needs to do
    */
    class IResponderHandshake : private openpal::Uncopyable
    {
    public:

        virtual ~IResponderHandshake() {}

        /**
        * Result type returned that indicates success/error along w/ reply data
        */
        class Result
        {

        public:


            HandshakeError error;
            seq32_t reply_data;

            inline static Result success(const seq32_t& reply_data)
            {
                return Result(HandshakeError::none, reply_data);
            }

            inline static Result failure(HandshakeError error)
            {
                return Result(error, seq32_t::empty());
            }

        private:

            Result(HandshakeError error, const seq32_t& reply_data) :
                error(error),
                reply_data(reply_data)
            {}

        };


        /**
        * Process a received handshake begin message. If no errors occur, use the frame writer to write a reply.
        *
        * @param msg the parsed RequestHandshakeBegin message
        * @param raw_data the raw bytes from which the message was parsed
        * @param now the time at which the message was received
        * @param writer interface used to write the reply to a buffer owned by the writer
        * @param session the session to initialize if the request is valid
        *
        * @return a result type that indicate success/error. On success, the reply_data is valid.
        */
        virtual Result process(const RequestHandshakeBegin& msg, const seq32_t& raw_data, const openpal::Timestamp& now, IFrameWriter& writer, Session& session) = 0;

    };



}

#endif

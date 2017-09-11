
#ifndef SSP21_IRESPONDERHANDSHAKE_H
#define SSP21_IRESPONDERHANDSHAKE_H

#include "ssp21/util/SequenceTypes.h"

#include "ssp21/crypto/gen/HandshakeError.h"
#include "ssp21/crypto/gen/RequestHandshakeBegin.h"

#include "ssp21/crypto/Session.h"


namespace ssp21
{

    /**
    * Class that holds all of the data used during the handshake process
    */
    class IResponderHandshake : private openpal::Uncopyable
    {
    public:

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

        virtual ~IResponderHandshake() {}

        virtual Result process(const RequestHandshakeBegin& msg, const seq32_t& raw_data, const openpal::Timestamp& now, IFrameWriter& writer, Session& session) = 0;

    };



}

#endif

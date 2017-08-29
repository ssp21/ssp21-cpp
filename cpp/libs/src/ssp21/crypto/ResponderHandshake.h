
#ifndef SSP21_RESPONDERHANDSHAKE_H
#define SSP21_RESPONDERHANDSHAKE_H

#include "ssp21/util/SequenceTypes.h"

#include "ssp21/crypto/gen/HandshakeError.h"
#include "ssp21/crypto/gen/RequestHandshakeBegin.h"

#include "ssp21/crypto/Session.h"
#include "ssp21/crypto/StaticKeys.h"
#include "ssp21/crypto/ICertificateHandler.h"

namespace ssp21
{

    /**
    * Class that holds all of the data used during the handshake process
    */
    class ResponderHandshake : private openpal::Uncopyable
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

		ResponderHandshake(openpal::Logger logger, StaticKeys static_keys, const std::shared_ptr<ICertificateHandler>& cert_handler);
        
		Result process(const RequestHandshakeBegin& msg, const seq32_t& raw_data, const openpal::Timestamp& now, IFrameWriter& writer, Session& session);

    private:        

        openpal::Logger logger;

		const StaticKeys static_keys;
		const std::shared_ptr<ICertificateHandler> cert_handler;
    };



}

#endif

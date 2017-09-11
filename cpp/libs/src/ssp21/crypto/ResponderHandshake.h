
#ifndef SSP21_RESPONDERHANDSHAKE_H
#define SSP21_RESPONDERHANDSHAKE_H

#include "ssp21/crypto/IResponderHandshake.h"


#include "ssp21/crypto/StaticKeys.h"
#include "ssp21/crypto/ICertificateHandler.h"

namespace ssp21
{

    /**
    * Class that holds all of the data used during the handshake process
    */
    class ResponderHandshake : public IResponderHandshake
    {
    public:

        ResponderHandshake(const openpal::Logger& logger, const StaticKeys& static_keys, const std::shared_ptr<ICertificateHandler>& cert_handler);

        inline static std::shared_ptr<IResponderHandshake> create_shared(openpal::Logger logger, StaticKeys static_keys, const std::shared_ptr<ICertificateHandler>& cert_handler)
        {
            return std::make_shared<ResponderHandshake>(logger, static_keys, cert_handler);
        }

        virtual IResponderHandshake::Result process(const RequestHandshakeBegin& msg, const seq32_t& raw_data, const openpal::Timestamp& now, IFrameWriter& writer, Session& session) override;

    private:

        openpal::Logger logger;

        const StaticKeys static_keys;
        const std::shared_ptr<ICertificateHandler> cert_handler;
    };



}

#endif

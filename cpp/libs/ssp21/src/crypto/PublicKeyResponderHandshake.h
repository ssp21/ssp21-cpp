
#ifndef SSP21_PUBLICKEYRESPONDERHANDSHAKE_H
#define SSP21_PUBLICKEYRESPONDERHANDSHAKE_H

#include "crypto/IResponderHandshake.h"


#include "ssp21/crypto/StaticKeys.h"
#include "ssp21/crypto/ICertificateHandler.h"

namespace ssp21
{
    class PublicKeyResponderHandshake : public IResponderHandshake
    {
    public:

        PublicKeyResponderHandshake(const log4cpp::Logger& logger, const StaticKeys& static_keys, const std::shared_ptr<ICertificateHandler>& cert_handler);

        inline static std::shared_ptr<IResponderHandshake> make_shared(log4cpp::Logger logger, StaticKeys static_keys, const std::shared_ptr<ICertificateHandler>& cert_handler)
        {
            return std::make_shared<PublicKeyResponderHandshake>(logger, static_keys, cert_handler);
        }

        virtual IResponderHandshake::Result process(const RequestHandshakeBegin& msg, const seq32_t& raw_data, const exe4cpp::steady_time_t& now, IFrameWriter& writer, Session& session) override;

    private:

        log4cpp::Logger logger;

        const StaticKeys static_keys;
        const std::shared_ptr<ICertificateHandler> cert_handler;
    };

}

#endif

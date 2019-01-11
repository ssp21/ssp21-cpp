
#ifndef SSP21_LOGLEVELS_H
#define SSP21_LOGLEVELS_H

#include "log4cpp/LogLevels.h"
#include "ser4cpp/util/Uncopyable.h"

namespace ssp21
{
    struct Module : ser4cpp::StaticOnly
    {
        // can be changed at runtime if there's ever a collision
        static log4cpp::ModuleId id;
    };


    namespace levels
    {
        // mirror the log4cpp levels

        const log4cpp::LogLevel event = log4cpp::levels::event;
        const log4cpp::LogLevel error = log4cpp::levels::error;
        const log4cpp::LogLevel warn = log4cpp::levels::warn;
        const log4cpp::LogLevel info = log4cpp::levels::info;
        const log4cpp::LogLevel debug = log4cpp::levels::debug;

        // custom log levels
        const log4cpp::LogLevel rx_crypto_msg = debug.next();
        const log4cpp::LogLevel tx_crypto_msg = rx_crypto_msg.next();
        const log4cpp::LogLevel rx_crypto_msg_fields = tx_crypto_msg.next();
        const log4cpp::LogLevel tx_crypto_msg_fields = rx_crypto_msg_fields.next();

    }
}

#endif


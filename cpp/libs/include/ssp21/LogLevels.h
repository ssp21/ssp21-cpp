
#ifndef SSP21_LOGLEVELS_H
#define SSP21_LOGLEVELS_H

#include "openpal/logging/LogLevels.h"
#include "openpal/util/Uncopyable.h"

namespace ssp21
{
    struct Module : openpal::StaticOnly
    {
        // can be changed at runtime if there's ever a collision
        static openpal::ModuleId id;
    };


    namespace levels
    {
// mirror the openpal levels

        const openpal::LogLevel event = openpal::levels::event;
        const openpal::LogLevel error = openpal::levels::error;
        const openpal::LogLevel warn = openpal::levels::warn;
        const openpal::LogLevel info = openpal::levels::info;
        const openpal::LogLevel debug = openpal::levels::debug;

// custom log levels
        const openpal::LogLevel rx_crypto_msg = debug.next();
        const openpal::LogLevel tx_crypto_msg = rx_crypto_msg.next();
        const openpal::LogLevel rx_crypto_msg_fields = tx_crypto_msg.next();
        const openpal::LogLevel tx_crypto_msg_fields = rx_crypto_msg_fields.next();

    }
}

#endif


#ifndef SSP21_LOGLEVELS_H
#define SSP21_LOGLEVELS_H

/** @file
 * @brief Logging levels.
 */

#include "log4cpp/LogLevels.h"
#include "ser4cpp/util/Uncopyable.h"

namespace ssp21
{
    struct Module : ser4cpp::StaticOnly
    {
        // can be changed at runtime if there's ever a collision
        static log4cpp::ModuleId id;
    };

    /**
     * @brief Logging levels
     */
    namespace levels
    {
        /// Event logs
        const log4cpp::LogLevel event = log4cpp::levels::event;
        /// Error logs
        const log4cpp::LogLevel error = log4cpp::levels::error;
        /// Warning logs
        const log4cpp::LogLevel warn = log4cpp::levels::warn;
        /// Information logs
        const log4cpp::LogLevel info = log4cpp::levels::info;
        /// Debug logs
        const log4cpp::LogLevel debug = log4cpp::levels::debug;

        /// RX logs
        const log4cpp::LogLevel rx_crypto_msg = debug.next();
        /// TX logs
        const log4cpp::LogLevel tx_crypto_msg = rx_crypto_msg.next();
        /// RX decoded message logs
        const log4cpp::LogLevel rx_crypto_msg_fields = tx_crypto_msg.next();
        /// TC decoded messages logs
        const log4cpp::LogLevel tx_crypto_msg_fields = rx_crypto_msg_fields.next();

    }
}

#endif

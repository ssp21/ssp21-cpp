
#include "ssp21/crypto/LogMessagePrinter.h"

#include "openpal/logging/LogMacros.h"
#include "openpal/logging/HexLogging.h"

using namespace openpal;

namespace ssp21
{

    LogMessagePrinter::LogMessagePrinter(const Logger& logger, LogLevel level, uint32_t max_hex_bytes_per_line) :
        logger_(logger),
        level_(level),
        max_hex_bytes_per_line_(max_hex_bytes_per_line)
    {

    }

    void LogMessagePrinter::print(const char* name, uint32_t value)
    {
        char message[max_log_entry_size];
        SAFE_STRING_FORMAT(message, max_log_entry_size, "%s: %u", name, value);
        logger_.log(level_, LOCATION, message);
    }

    void LogMessagePrinter::print(const char* name, const char* value)
    {
        char message[max_log_entry_size];
        SAFE_STRING_FORMAT(message, max_log_entry_size, "%s: %s", name, value);
        logger_.log(level_, LOCATION, message);
    }

    void LogMessagePrinter::print(const char* name, const RSlice& data)
    {
        char message[max_log_entry_size];
        SAFE_STRING_FORMAT(message, max_log_entry_size, "%s (length = %u)", name, data.length());
        logger_.log(level_, LOCATION, message);

        HexLogging::log(logger_, level_, data, ':', max_hex_bytes_per_line_, max_hex_bytes_per_line_);
    }

    void LogMessagePrinter::print(const char* name, const Seq8Seq16& items)
    {
        char message[max_log_entry_size];

        SAFE_STRING_FORMAT(message, max_log_entry_size, "%s (count = %u)", name, items.count());
        logger_.log(level_, LOCATION, message);

        for (uint32_t i = 0; i < items.count(); ++i)
        {
            RSlice item;
            items.read(i, item);

            SAFE_STRING_FORMAT(message, max_log_entry_size, "#%u (length = %u)", i + 1, item.length());
            logger_.log(level_, LOCATION, message);

            HexLogging::log(logger_, level_, item, ':', max_hex_bytes_per_line_, max_hex_bytes_per_line_);
        }
    }

}



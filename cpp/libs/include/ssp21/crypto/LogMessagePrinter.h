
#ifndef SSP21_LOGMESSAGEPRINTER_H
#define SSP21_LOGMESSAGEPRINTER_H

#include "ssp21/crypto/IMessagePrinter.h"

#include "openpal/logging/Logger.h"
#include "openpal/util/Uncopyable.h"

namespace ssp21
{

    /**
    * A simple interface for pretty prininting
    * complex messages as a sequence of lines
    */
    class LogMessagePrinter : public IMessagePrinter, private openpal::Uncopyable
    {

    public:

        LogMessagePrinter(const openpal::Logger& logger, openpal::LogLevel level, uint32_t max_hex_bytes_per_line = 20);

        virtual void print(const char* message) override;
        virtual void print(const char* name, uint32_t value) override;
        virtual void print(const char* name, const char* value) override;
        virtual void print(const char* name, const seq32_t& data) override;

    private:



        openpal::Logger logger_;
        openpal::LogLevel level_;
        const uint32_t max_hex_bytes_per_line_;
    };

}

#endif

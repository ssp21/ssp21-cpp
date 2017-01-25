
#ifndef SSP21_CONSOLEPRETTYPRINTER_H
#define SSP21_CONSOLEPRETTYPRINTER_H

#include "openpal/logging/ILogHandler.h"
#include <cstddef>

namespace ssp21
{

    /**
    * pretty prints SSP21 log messages
    */
    class ConsolePrettyPrinter final : public openpal::ILogHandler
    {

    public:

        class Settings
        {

        public:
            Settings() {}

            bool printId = true;
            size_t max_id_size = 10;
        };

        ConsolePrettyPrinter(const Settings& settings = Settings()) : settings(settings) {}

        virtual void log(openpal::ModuleId module, const char* id, openpal::LogLevel level, char const* location, char const* message) override;

    private:

        const Settings settings;

        static const char* get_prefix(int level);
    };

}

#endif

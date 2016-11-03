#ifndef SSP21_MOCKLOGHANDLER_H
#define SSP21_MOCKLOGHANDLER_H

#include "openpal/logging/Logger.h"
#include "openpal/util/Uncopyable.h"

#include "ssp21/LogLevels.h"

#include "catch.hpp"

#include <deque>
#include <iostream>

namespace ssp21
{
    class MockLogHandler : public openpal::ILogHandler, openpal::Uncopyable
    {
        struct Backend final : public openpal::ILogHandler
        {
            void log(openpal::ModuleId module, const char* id, openpal::LogLevel level, char const* location, char const* message) override
            {
                if (output_to_stdio)
                {
                    std::cout << message << std::endl;
                }

                lines.push_back(message);
            }

            bool output_to_stdio = false;
            std::deque<std::string> lines;
        };

    public:

        MockLogHandler(const std::string& id, openpal::LogLevels levels = openpal::LogLevels::everything()) :
            backend(std::make_shared<Backend>()),
            logger(backend, openpal::ModuleId(0), id, levels)
        {}

        void print_output()
        {
            this->backend->output_to_stdio = true;
        }

        virtual void log(openpal::ModuleId module, const char* id, openpal::LogLevel level, char const* location, char const* message) override
        {
            this->backend->log(module, id, level, location, message);
        }

        template <typename... Args>
        void expect(const std::string& expected, const Args& ... args)
        {
            REQUIRE_FALSE(backend->lines.empty());
            REQUIRE(expected == backend->lines.front());
            backend->lines.pop_front();
            expect(args ...);
        }

        void expect()
        {
            REQUIRE(backend->lines.empty());
        }

    private :

        std::shared_ptr<Backend> backend;

        MockLogHandler() = delete;

    public:

        openpal::Logger logger;

    };

}

#endif
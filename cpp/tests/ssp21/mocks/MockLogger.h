#ifndef SSP21_MOCKLOGGER_H
#define SSP21_MOCKLOGGER_H

#include "openpal/logging/LogRoot.h"
#include "openpal/util/Uncopyable.h"

#include "ssp21/LogLevels.h"

#include "catch.hpp"

#include <deque>
#include <iostream>

namespace ssp21
{
    class MockLogger : public openpal::ILogHandler, openpal::Uncopyable
    {

    public:
        MockLogger(const char* id, openpal::LogLevels levels = openpal::LogLevels::everything()) :
            root(ssp21::Module::id, this, id, levels),
            print_output_(false)
        {}

        void print_output()
        {
            print_output_ = true;
        }

        virtual void log(openpal::ModuleId module, const char* id, openpal::LogLevel level, char const* location, char const* message)  override
        {
            if (print_output_)
            {
                std::cout << message << std::endl;
            }

            lines.push_back(message);
        }

        template <typename... Args>
        void validate(const std::string& expected, const Args& ... args)
        {
            REQUIRE_FALSE(lines.empty());
            REQUIRE(expected == lines.front());
            lines.pop_front();
            validate(args ...);
        }

        void validate()
        {
            REQUIRE(lines.empty());
        }

        std::deque<std::string> lines;
        openpal::LogRoot root;

    private :

        bool print_output_;

        MockLogger() = delete;

    };

}

#endif
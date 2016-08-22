#ifndef SSP21_MOCKLOGGER_H
#define SSP21_MOCKLOGGER_H

#include "openpal/logging/LogRoot.h"
#include "openpal/util/Uncopyable.h"

#include "catch.hpp"

#include <deque>

namespace ssp21
{
	class MockLogger : public openpal::ILogHandler, openpal::Uncopyable
	{

	public:
		MockLogger(const char* id, openpal::LogFilters filters) : root(this, id , filters)
		{}
		
		virtual void log(const openpal::LogEntry &entry)  override
		{
			lines.push_back(entry.get_message());
		}

		template <typename... Args>
		void validate(const std::string& expected, const Args& ... args)
		{
			REQUIRE_FALSE(lines.empty());
			REQUIRE(expected == lines.front());
			lines.pop_front();
			validate(args ...);
		}

		void validate() {
			REQUIRE(lines.empty());
		}

		std::deque<std::string> lines;
		openpal::LogRoot root;

	private :

		MockLogger() = delete;

	};

}

#endif
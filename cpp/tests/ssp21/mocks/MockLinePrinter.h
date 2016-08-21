
#ifndef SSP21_MOCKLINEPRINTER_H
#define SSP21_MOCKLINEPRINTER_H

#include "catch.hpp"
#include "ssp21/ILinePrinter.h"

#include <deque>

namespace ssp21 {

struct MockLinePrinter final : public ILinePrinter
{
	virtual void print(const char* line) override
	{
		lines.push_back(line);
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
};

}

#endif

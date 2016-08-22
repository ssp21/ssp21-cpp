
#ifndef SSP21_LOGLINEPRINTER_H
#define SSP21_LOGLINEPRINTER_H

#include "ssp21/ILinePrinter.h"

#include "openpal/logging/Logger.h"

namespace ssp21 {

/**
* A simple interface for pretty prininting
* complex messages as a sequence of lines
*/
class LogLinePrinter : public ILinePrinter, private openpal::Uncopyable
{

public:

	LogLinePrinter(const openpal::Logger& logger, openpal::LogFilters filters);

	virtual void print(const char* name, uint32_t value) override;
	virtual void print(const char* name, const char* value) override;
	virtual void print(const char* name, const openpal::RSlice& data) override;
	virtual void print(const char* name, const Seq8Seq16& items) override;

private:

	openpal::Logger logger_;
	openpal::LogFilters filters_;
};

}

#endif

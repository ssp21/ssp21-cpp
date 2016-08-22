
#ifndef SSP21_ILINEPRINTER_H
#define SSP21_ILINEPRINTER_H

#include "ssp21/SequenceTypes.h"

namespace ssp21 {

/**
* A simple interface for pretty prininting
* complex messages as a sequence of lines
*/
class ILinePrinter
{

public:

	virtual void print(const char* name, uint32_t value) = 0;
	virtual void print(const char* name, const char* value) = 0;	
	virtual void print(const char* name, const openpal::RSlice& data) = 0;
	virtual void print(const char* name, const Seq8Seq16& items) = 0;

};

}

#endif

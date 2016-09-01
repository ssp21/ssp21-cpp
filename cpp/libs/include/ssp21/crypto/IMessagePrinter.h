
#ifndef SSP21_IMESSAGEPRINTER_H
#define SSP21_IMESSAGEPRINTER_H

#include "openpal/container/RSlice.h"

namespace ssp21
{

    /**
    * A simple interface for pretty printing
    * complex messages as a sequence of
    */
    class IMessagePrinter
    {

    public:

		virtual void print(const char* message) = 0;
        virtual void print(const char* name, uint32_t value) = 0;
        virtual void print(const char* name, const char* value) = 0;
        virtual void print(const char* name, const openpal::RSlice& value) = 0;        
    };

	/**
	* Any type that can print contents to an IMessagePrinter
	*/
	class IPrintable
	{

	public:

		virtual void print(const char* name, IMessagePrinter& printer) const = 0;

	};

}

#endif

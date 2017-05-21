#ifndef SSP21_PEMFORMAT_H
#define SSP21_PEMFORMAT_H

#include "openpal/util/Uncopyable.h"

#include "ssp21/crypto/gen/PEMDecodeError.h"

#include <string>
#include <fstream>

#define PEM_BEGIN_DELIM_START "-----BEGIN "
#define PEM_DELIM_END "-----"

namespace ssp21
{
    int constexpr length(const char* str)
    {
        return *str ? 1 + length(str + 1) : 0;
    }

    class PEMFormat : private openpal::StaticOnly
    {		

    public:

        struct IHandler
        {
            virtual ~IHandler() {}

            // return false to halt decoding
            virtual bool on_section(const std::string& id, const char* line) = 0;
        };

		struct IReader
		{
			virtual ~IReader() {}

			// return false to halt decoding
			virtual size_t read_line(char* dest, size_t max_characters) = 0;
		};

        static PEMDecodeError decode(IReader& reader, IHandler& handler, std::error_code& ec);

        static bool get_section(const std::string& line, std::string& id);

    private:

        static const std::string begin_delim;

        static const int begin_delim_length = length(PEM_BEGIN_DELIM_START);
        static const int trailer_delim_length = length(PEM_DELIM_END);
        static const int total_begin_line_delim_length = begin_delim_length + trailer_delim_length;

    };

}

#endif

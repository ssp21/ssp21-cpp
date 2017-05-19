#ifndef SSP21_PEMFORMAT_H
#define SSP21_PEMFORMAT_H

#include "openpal/util/Uncopyable.h"

#include "ssp21/crypto/gen/PEMDecodeError.h"

#include <string>
#include <fstream>

#define MACRO_PEM_BEGIN_DELIM_START "-----BEGIN "
#define MACRO_PEM_DELIM_END "-----"

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

            /// return false to halt decoding
            virtual bool on_section(const std::string& id, const std::string& base64) = 0;
        };

        static PEMDecodeError decode(std::ifstream& input, IHandler& handler, std::error_code& ec);

        static bool get_section(const std::string& line, std::string& id);

    private:

        static const std::string begin_delim;

        static const int begin_delim_length = length(MACRO_PEM_BEGIN_DELIM_START);
        static const int trailer_delim_length = length(MACRO_PEM_DELIM_END);
        static const int total_begin_line_delim_length = begin_delim_length + trailer_delim_length;

    };

}

#endif

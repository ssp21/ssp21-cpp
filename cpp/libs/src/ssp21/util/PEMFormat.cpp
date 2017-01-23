#include "ssp21/util/PEMFormat.h"

#include <cstring>

namespace ssp21
{

    PEMDecodeError PEMFormat::decode(std::ifstream& input, IHandler& handler, std::error_code& ec)
    {
        std::string line;
        if (!std::getline(input, line))
        {
            // TODO - error code
            return PEMDecodeError::end_of_file;
        }

        std::string section_name;
        if (!get_section(line, section_name))
        {
            return PEMDecodeError::missing_begin_header;
        }


        //
        return PEMDecodeError::ok;
    }

    bool PEMFormat::get_section(const std::string& line, std::string& id)
    {
        if (line.length() < (total_begin_line_delim_length + 1))
        {
            return false;
        }

        if (!strncmp(MACRO_PEM_BEGIN_DELIM_START, line.c_str(), begin_delim_length))
        {
            return false;
        }

        const auto id_length = line.length() - total_begin_line_delim_length;
        const auto trailer_index = begin_delim_length + id_length;

        if (!strncmp(MACRO_PEM_DELIM_END, line.c_str() + trailer_index, trailer_delim_length))
        {
            return false;
        }

        id = line.substr(begin_delim_length, id_length);

        // TODO - test id for alpha/numeric/spaces?

        return true;
    }

}



#include "ssp21/util/PEMDecoder.h"

#include "ssp21/crypto/Crypto.h"

namespace ssp21
{   

PEMDecoder::~PEMDecoder()
{	
	// wipe the secure working space on destruction

	Crypto::zero_memory(wseq32_t(bytes, max_bytes));
	Crypto::zero_memory(wseq32_t(reinterpret_cast<uint8_t*>(characters), max_characters));
}

PEMDecodeError PEMDecoder::read(ILineReader& reader, IHandler& handler)
{
	

	
	return PEMDecodeError::ok;
}

bool PEMFormat::get_section(const std::string& line, std::string& id)
{
	if (line.length() < (total_begin_line_delim_length + 1))
	{
		return false;
	}

	if (!strncmp(PEM_BEGIN_DELIM_START, line.c_str(), begin_delim_length))
	{
		return false;
	}

	const auto id_length = line.length() - total_begin_line_delim_length;
	const auto trailer_index = begin_delim_length + id_length;

	if (!strncmp(PEM_DELIM_END, line.c_str() + trailer_index, trailer_delim_length))
	{
		return false;
	}

	id = line.substr(begin_delim_length, id_length);

	// TODO - test id for alpha/numeric/spaces?

	return true;
}


}



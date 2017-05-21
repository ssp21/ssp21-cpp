
#include "ssp21/util/SecureFileReader.h"

#include "ssp21/crypto/Crypto.h"

#include <fstream>

namespace ssp21
{

SecureFileReader::~SecureFileReader()
{
	if (this->buffer)
	{
		Crypto::zero_memory(this->buffer->as_wslice());
	}
}

seq32_t SecureFileReader::read(const std::string& path)
{
	if (this->buffer)
	{
		Crypto::zero_memory(this->buffer->as_wslice());
		this->buffer.reset();
	}

	std::ifstream stream(path, std::ios::binary | std::ios::ate);
	const std::streamsize size = stream.tellg();

	if (size < 0 || size > maximum_file_size) {		
		return seq32_t::empty();
	}	

	stream.seekg(0, std::ios::beg);
	this->buffer = std::make_unique<openpal::Buffer>(static_cast<uint32_t>(size));

	char* dest = reinterpret_cast<char*>(*this->buffer->as_wslice());
	
	if (stream.read(dest, size))
	{
		return this->buffer->as_rslice();
	}
	else
	{
		return seq32_t::empty();
	}	
}

}


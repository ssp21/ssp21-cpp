
#ifndef SSP21_SECUREFILEREADER_H
#define SSP21_SECUREFILEREADER_H

#include "ssp21/util/SequenceTypes.h"

#include <openpal/container/Buffer.h>
#include <string>
#include <memory>


namespace ssp21
{

class SecureFileReader
{
	std::unique_ptr<openpal::Buffer> buffer;

	static const size_t default_maximum_file_size = 4096;

	size_t maximum_file_size;

public:

	SecureFileReader(size_t maximum_file_size = default_maximum_file_size) : maximum_file_size(maximum_file_size)
	{}

	~SecureFileReader();

	seq32_t read(const std::string& path);

};


}

#endif

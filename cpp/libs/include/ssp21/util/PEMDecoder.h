#ifndef SSP21_PEMDECODER_H
#define SSP21_PEMDECODER_H

#include "PEMFormat.h"
#include "ssp21/util/SequenceTypes.h"

namespace ssp21
{   
    class PEMDecoder : private openpal::Uncopyable
    {
		// maximum number of encoded bytes per section
		static const size_t max_bytes = 4096; 
		
		// maximum number of base64 characters per line
		static const size_t max_line_length = 64;
		
		// maximum number of characters required to encode maximum # of bytes
		static const size_t max_characters = ((max_bytes * 4) / 3) + (((max_bytes * 4) % 3) == 0 ? 0 : 1);

		char characters[max_characters];
		uint8_t bytes[max_bytes];		

    public:

		~PEMDecoder();

		struct IHandler
		{
			virtual ~IHandler() {}

			// return false to halt decoding
			virtual bool on_section(const std::string& id, const seq32_t& data) = 0;
		};

		struct ILineReader
		{
			virtual ~ILineReader() {}

			// return false to halt decoding
			virtual size_t read_line(char* dest, size_t max_characters) = 0;
		};

		PEMDecodeError read(ILineReader& reader, IHandler& handler);
    };

}

#endif

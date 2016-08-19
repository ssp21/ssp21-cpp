
#include <cstdint>

#include <iostream>
#include <iomanip>

using namespace std;


// koopman
// typedef uint32_t crc_t;
// const crc_t polynomial = 0x32583499;


// Castagnoli
typedef uint32_t crc_t;
const crc_t polynomial = 0xF4ACFB13;

// DNP3
// typedef uint16_t crc_t;
// const crc_t polynomial = 0x3D65;

const int width = (8 * sizeof(crc_t));
const crc_t topbit = (1 << (width - 1));
const int output_width = 2 * sizeof(crc_t);

/*
	Created to match:
	
	http://www.sunshine2k.de/coding/javascript/crc/crc_js.html
*/
int main(int argc, char*  argv[])
{	
	crc_t table[256];
	
	for (uint32_t i = 0; i < 256; ++i) { // foreach possible byte value
		
		uint32_t remainder = i << (width - 8);

		for (int bit = 8; bit > 0; --bit) 
		{
			if (remainder & topbit)
				remainder = (remainder << 1) ^ polynomial;
			else
				remainder <<= 1;
		}
		table[i] = remainder;
	}		 

	for (int i = 0; i < 256; ++i)
	{		
		std::cout << "0x" << std::setfill('0') << std::setw(output_width) << std::hex << table[i];

		if (i % 8 == 7) {
			std::cout << "," << std::endl;
		}
		else {
			std::cout << ", ";
		}		
	}

	// default test vector from website above
	const uint8_t input[9] = { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39 };

		
	crc_t remainder = 0;

	for (uint32_t i = 0; i < 9; ++i)
	{
		uint8_t index = input[i] ^ (remainder >> (width - 8));
		remainder = table[index] ^ (remainder << 8);
	}

	std::cout << std::endl << "CRC: 0x" << remainder << std::endl;
	

	return 0;
}

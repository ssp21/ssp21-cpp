
#include <cstdint>

#include <iostream>
#include <iomanip>

using namespace std;

template <class T>
T to_msb_first(typename T koopman)
{
	return ((koopman << 1) | 1);
}

template <class T>
T to_koopman(typename T msb_first)
{
	return ((msb_first >> 1) | (1 << (sizeof(T)*8 - 1))) ;
}

template <class crc_t>
struct Polynomial
{	
	static const int width = (8 * sizeof(crc_t));
	static const crc_t top_bit = (1 << (width - 1));
	static const crc_t bottom_bit = (1 << (width - 1));
	static const int output_width = 2 * sizeof(crc_t);

	Polynomial(crc_t polynomial) : polynomial(polynomial)
	{}

	void print()
	{
		std::cout << "msb first: " << "0x" << std::setfill('0') << std::setw(output_width) << std::hex << polynomial << std::endl;
		std::cout << "koopman:   " << "0x" << std::setfill('0') << std::setw(output_width) << std::hex << to_koopman<crc_t>(polynomial) << std::endl;

		crc_t table[256];

		for (uint32_t i = 0; i < 256; ++i) { // foreach possible byte value

			uint32_t remainder = i << (width - 8);

			for (int bit = 8; bit > 0; --bit)
			{
				if (remainder & top_bit)
					remainder = (remainder << 1) ^ polynomial;
				else
					remainder <<= 1;
			}
			table[i] = remainder;
		}

		const uint8_t input[9] = { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39 };


		crc_t remainder = 0;

		for (uint32_t i = 0; i < 9; ++i)
		{
			uint8_t index = input[i] ^ (remainder >> (width - 8));
			remainder = table[index] ^ (remainder << 8);
		}

		std::cout << "test vector: " << "0x" << std::setfill('0') << std::setw(output_width) << std::hex << remainder << std::endl;

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
	}

	const crc_t polynomial;
};


/*
	Created to match:
	
	http://www.sunshine2k.de/coding/javascript/crc/crc_js.html
*/
int main(int argc, char*  argv[])
{	
	Polynomial<uint32_t> koopman(to_msb_first<uint32_t>(0x992C1A4C));
	Polynomial<uint32_t> castagnoli(to_msb_first<uint32_t>(0xFA567D89));
	Polynomial<uint32_t> ethernet(to_msb_first<uint32_t>(0x82608EDB));
	Polynomial<uint16_t> dnp3(0x3D65);

	koopman.print();
	castagnoli.print();
	ethernet.print();
	dnp3.print();
	
	return 0;
}


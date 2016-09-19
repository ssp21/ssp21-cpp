#ifndef SSP21_BASE64_H
#define SSP21_BASE64_H

#include "openpal/util/Uncopyable.h"
#include "openpal/container/RSlice.h"

#include <cstdint>
#include <functional>

namespace ssp21
{
    class Base64 : private openpal::StaticOnly
    {

    public:

		template <class CharWriteFun>
		static void encode(const openpal::RSlice& bytes, const CharWriteFun& write)
		{
			uint32_t pos = 0;			

			while (pos < bytes.length())
			{				
				const auto cursor = bytes.skip(pos);				

				switch (cursor.length())
				{
				case(1):
					write(get_first_char(cursor[0]));
					write(get_second_char(cursor[0], 0));
					write('=');
					write('=');
					break;
				case(2):
					write(get_first_char(cursor[0]));
					write(get_second_char(cursor[0], cursor[1]));
					write(get_third_char(cursor[1], 0));
					write('=');
					break;
				default:
					write(get_first_char(cursor[0]));
					write(get_second_char(cursor[0], cursor[1]));
					write(get_third_char(cursor[1], cursor[2]));
					write(get_fourth_char(cursor[2]));
					break;
				}

				pos += 3;
			};
		};

    private:

        inline static char get_first_char(uint8_t first)
        {
            return codes[((first & 0b11111100) >> 2)];
        }

        inline static char get_second_char(uint8_t first, uint8_t second)
        {
            return codes[((first & 0b00000011) << 4) | ((second & 0b11110000) >> 4)];
        }

        inline static char get_third_char(uint8_t second, uint8_t third)
        {
            return codes[((second & 0b00001111) << 2) | ((third & 0b11000000) >> 6)];
        }

        inline static char get_fourth_char(uint8_t third)
        {
            return codes[(third & 0b00111111)];
        }


        static const char codes[64];
    };

}

#endif

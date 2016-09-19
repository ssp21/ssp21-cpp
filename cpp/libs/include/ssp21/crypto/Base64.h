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

        enum class DecodeError : uint8_t
        {
            // decode sucess
            ok,
            // The non-whitespace input is not a multiple of four
            not_mult_four,
            // The input contains a non-base64 value
            not_base64,
            // The input contains non-whitespace characters after the terminating padding
            bad_end_char,
        };

        /// Takes an input byte slice and outputs one encoded character at a time to an arbitrary lambda
        /// accepting a char
        template <class CharWriteFun>
		static void encode(const openpal::RSlice& bytes, const CharWriteFun& write);		

        template <class ByteWriteFun>
		static DecodeError decode(const openpal::RSlice& chars, const ByteWriteFun& write);

    private:

        template <typename T, typename... Args>
        static bool any_not_base64(T value, Args& ... args)
        {
            return (value == not_base64_char) || any_not_base64(args...);
        }

        static bool any_not_base64()
        {
            return false;
        }

        struct DecodeChars
        {
            uint8_t char1 = 0;
            uint8_t char2 = 0;
            uint8_t char3 = 0;
            uint8_t char4 = 0;
        };

        class DecodeCursor
        {
        public:

            DecodeCursor(const openpal::RSlice& chars) : pos(chars)
            {}

            DecodeError get_next_chars(DecodeChars& chars)
            {
                auto err = get_next_char(chars.char1);
                if (err != DecodeError::ok) return err;

                err = get_next_char(chars.char2);
                if (err != DecodeError::ok) return err;

                err = get_next_char(chars.char3);
                if (err != DecodeError::ok) return err;

                err = get_next_char(chars.char4);
                return err;
            }

            bool is_empty() const
            {
                return pos.is_empty();
            }

        private:

            inline static bool is_whitespace(uint8_t c)
            {
                switch (c)
                {
                case('\n'):
                case('\r'):
                case('\t'):
                case(' '):
                    return true;
                default:
                    return false;
                }
            }

            DecodeError get_next_char(uint8_t& value)
            {
                while (!pos.is_empty())
                {
                    const auto raw_value = pos[0];
                    pos.advance(1);

                    if (!is_whitespace(raw_value))
                    {
                        value = raw_value;
                        return DecodeError::ok;
                    }
                }

                return DecodeError::not_mult_four;
            }

            openpal::RSlice pos;
        };

        inline static char get_first_char(uint8_t first)
        {
            return encode_table[((first & 0b11111100) >> 2)];
        }

        inline static char get_second_char(uint8_t first, uint8_t second)
        {
            return encode_table[((first & 0b00000011) << 4) | ((second & 0b11110000) >> 4)];
        }

        inline static char get_third_char(uint8_t second, uint8_t third)
        {
            return encode_table[((second & 0b00001111) << 2) | ((third & 0b11000000) >> 6)];
        }

        inline static char get_fourth_char(uint8_t third)
        {
            return encode_table[(third & 0b00111111)];
        }

        inline static uint8_t get_first_byte(uint8_t b1, uint8_t b2)
        {
            return ((b1 & 0b00111111) << 2) | ((b2 & 0b00110000) >> 4);
        }

        inline static uint8_t get_second_byte(uint8_t b2, uint8_t b3)
        {
            return ((b2 & 0b00001111) << 4) | ((b3 & 0b00111100) >> 2);
        }

        inline static uint8_t get_third_byte(uint8_t b3, uint8_t b4)
        {
            return ((b3 & 0b00000011) << 6) | (b4 & 0b00111111);
        }

        static const char encode_table[64];

        // special value in the decode table that says the corresponding char isn't base64
        static const uint8_t not_base64_char = 0xFF;

        // index by character value (0 .. 255).
        static const uint8_t decode_table[256];
    };

	template <class CharWriteFun>
	void Base64::encode(const openpal::RSlice& bytes, const CharWriteFun& write)
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
		}
	}

	template <class ByteWriteFun>
	Base64::DecodeError Base64::decode(const openpal::RSlice& chars, const ByteWriteFun& write)
	{
		DecodeCursor cursor(chars);

		while (!cursor.is_empty())
		{
			DecodeChars chars;

			auto err = cursor.get_next_chars(chars);
			if (err != DecodeError::ok) return err;

			if (chars.char4 == '=')
			{
				if (chars.char3 == '=')
				{
					// two trailing "="
					auto v1 = decode_table[chars.char1];
					auto v2 = decode_table[chars.char2];
					if (any_not_base64(v1, v2)) return DecodeError::not_base64;
					write(get_first_byte(v1, v2));
					return cursor.is_empty() ? DecodeError::ok : DecodeError::bad_end_char;
				}
				else
				{
					// single trailing "="
					auto v1 = decode_table[chars.char1];
					auto v2 = decode_table[chars.char2];
					auto v3 = decode_table[chars.char3];
					if (any_not_base64(v1, v2, v3)) return DecodeError::not_base64;
					write(get_first_byte(v1, v2));
					write(get_second_byte(v2, v3));
					return cursor.is_empty() ? DecodeError::ok : DecodeError::bad_end_char;
				}
			}
			else
			{
				auto v1 = decode_table[chars.char1];
				auto v2 = decode_table[chars.char2];
				auto v3 = decode_table[chars.char3];
				auto v4 = decode_table[chars.char4];
				if (any_not_base64(v1, v2, v3, v4)) return DecodeError::not_base64;
				write(get_first_byte(v1, v2));
				write(get_second_byte(v2, v3));
				write(get_third_byte(v3, v4));
			}
		}

		return DecodeError::ok;
	};

}

#endif
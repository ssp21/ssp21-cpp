#ifndef SSP21_BASE64_H
#define SSP21_BASE64_H

#include "ssp21/crypto/gen/Base64DecodeError.h"

#include "openpal/util/Uncopyable.h"
#include "openpal/container/RSlice.h"

#include <cstdint>
#include <functional>

namespace ssp21
{
    class Base64 : private openpal::StaticOnly
    {

    public:

        /// Takes an input byte slice and outputs one encoded character at a time to an arbitrary lambda
        /// accepting a char
        template <class CharWriteFun>
        static void encode(const openpal::RSlice& bytes, const CharWriteFun& write);

        /// Consumes a slice of base64 characters and emits a sequence of bytes to an arbitrary lambda
        /// Ignores whitespace characters.
        template <class ByteWriteFun>
        static Base64DecodeError decode(const openpal::RSlice& chars, const ByteWriteFun& write);

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

            Base64DecodeError get_next_chars(DecodeChars& chars);

            bool is_empty() const
            {
                return pos.is_empty();
            }


            void trim_leading_whitespace();

        private:

            static bool is_whitespace(uint8_t c);

            Base64DecodeError get_next_char(uint8_t& value);

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
    Base64DecodeError Base64::decode(const openpal::RSlice& chars, const ByteWriteFun& write)
    {
        DecodeCursor cursor(chars);

        cursor.trim_leading_whitespace();

        while (!cursor.is_empty())
        {
            DecodeChars chars;

            auto err = cursor.get_next_chars(chars);
            if (any(err)) return err;

            cursor.trim_leading_whitespace();

            if (chars.char4 == '=')
            {
                if (chars.char3 == '=')
                {
                    // two trailing "="
                    auto v1 = decode_table[chars.char1];
                    auto v2 = decode_table[chars.char2];
                    if (any_not_base64(v1, v2)) return Base64DecodeError::not_base64;
                    write(get_first_byte(v1, v2));
                    return cursor.is_empty() ? Base64DecodeError::ok : Base64DecodeError::bad_trailing_input;
                }
                else
                {
                    // single trailing "="
                    auto v1 = decode_table[chars.char1];
                    auto v2 = decode_table[chars.char2];
                    auto v3 = decode_table[chars.char3];
                    if (any_not_base64(v1, v2, v3)) return Base64DecodeError::not_base64;
                    write(get_first_byte(v1, v2));
                    write(get_second_byte(v2, v3));
                    return cursor.is_empty() ? Base64DecodeError::ok : Base64DecodeError::bad_trailing_input;
                }
            }
            else
            {
                auto v1 = decode_table[chars.char1];
                auto v2 = decode_table[chars.char2];
                auto v3 = decode_table[chars.char3];
                auto v4 = decode_table[chars.char4];
                if (any_not_base64(v1, v2, v3, v4)) return Base64DecodeError::not_base64;
                write(get_first_byte(v1, v2));
                write(get_second_byte(v2, v3));
                write(get_third_byte(v3, v4));
            }
        }

        return Base64DecodeError::ok;
    };

}

#endif

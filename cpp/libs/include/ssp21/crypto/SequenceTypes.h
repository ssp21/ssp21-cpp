
#ifndef SSP21_SEQUENCETYPES_H
#define SSP21_SEQUENCETYPES_H

#include "openpal/util/Uncopyable.h"

#include "openpal/container/RSlice.h"
#include "openpal/container/WSlice.h"

#include "ssp21/crypto/gen/FormatError.h"
#include "ssp21/crypto/gen/ParseError.h"
#include "ssp21/crypto/Constants.h"


namespace ssp21
{
    class IMessagePrinter;

    class Seq8 final : public openpal::RSeq<uint8_t, uint8_t, Seq8>
    {
    public:

        Seq8() {}

        static Seq8 from(const openpal::RSlice& data, uint8_t length);

        Seq8(uint8_t const* buffer, uint8_t length) : RSeq(buffer, length)
        {}

        operator openpal::RSlice() const
        {
            return openpal::RSlice(buffer_, length_);
        }

        ParseError read(openpal::RSlice& input);
        FormatError write(openpal::WSlice& output) const;
        void print(const char* name, IMessagePrinter& printer) const;
    };

    class Seq16 final : public openpal::RSeq<uint8_t, uint16_t, Seq16>
    {
    public:

        Seq16() {}

        static Seq16 from(const openpal::RSlice& data, uint16_t length);

        Seq16(uint8_t const* buffer, uint16_t length) : RSeq(buffer, length)
        {}

        openpal::RSlice as_rslice() const
        {
            return openpal::RSlice(buffer_, length_);
        }

        operator openpal::RSlice() const
        {
            return openpal::RSlice(buffer_, length_);
        }

        ParseError read(openpal::RSlice& input);
        FormatError write(openpal::WSlice& output) const;
        void print(const char* name, IMessagePrinter& printer) const;

    };

    class Seq8Seq16
    {

    public:

        Seq8Seq16();

        ParseError read(openpal::RSlice& input);
        FormatError write(openpal::WSlice& output) const;
        void print(const char* name, IMessagePrinter& printer) const;

        void clear();

        bool push(const Seq16& slice);

        bool read(uint32_t i, Seq16& slice) const;

        uint8_t count() const;

    private:

        uint8_t count_;
        Seq16 slices_[consts::crypto::max_seq_of_seq];
    };

}

#endif

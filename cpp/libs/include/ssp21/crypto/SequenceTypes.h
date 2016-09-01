
#ifndef SSP21_SEQUENCETYPES_H
#define SSP21_SEQUENCETYPES_H

#include <openpal/util/Uncopyable.h>

#include <openpal/container/RSlice.h>

#include "ssp21/crypto/Constants.h"
#include "ssp21/crypto/IMessagePrinter.h"
#include "ssp21/crypto/IReadable.h"
#include "ssp21/crypto/IWritable.h"

namespace ssp21
{
    class Seq8 final : public openpal::RSlice, public IReadable, public IWritable, public IPrintable
    {
    public:

        Seq8() : openpal::RSlice()
        {}

		virtual ParseError read(openpal::RSlice& input) override;
		virtual FormatError write(openpal::WSlice& output) const override;
		virtual void print(const char* name, IMessagePrinter& printer) const override;
		


        explicit Seq8(const openpal::RSlice& other) : RSlice(other)
        {}
    };

    class Seq16 final : public openpal::RSlice, public IReadable, public IWritable, public IPrintable
    {
    public:

        Seq16() : openpal::RSlice()
        {}

		virtual ParseError read(openpal::RSlice& input) override;
		virtual FormatError write(openpal::WSlice& output) const override;
		virtual void print(const char* name, IMessagePrinter& printer) const override;

        explicit Seq16(const openpal::RSlice& other) : RSlice(other)
        {}
    };

    class SeqRSlice : public IReadable, public IWritable, public IPrintable
    {

    public:

        SeqRSlice();

		virtual ParseError read(openpal::RSlice& input) override;
		virtual FormatError write(openpal::WSlice& output) const override;
		virtual void print(const char* name, IMessagePrinter& printer) const override;

        void clear();

        bool push(const openpal::RSlice& slice);

        bool read(uint32_t i, openpal::RSlice& slice) const;

        uint32_t count() const;

    private:

        uint32_t count_;
        openpal::RSlice slices_[consts::crypto::max_seq_of_seq];
    };

    // specialized types with type dependent constraints
    class Seq8Seq16 final : public SeqRSlice { };
}

#endif

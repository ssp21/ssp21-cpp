
#ifndef SSP21_SEQUENCEFIELDTYPES_H
#define SSP21_SEQUENCEFIELDTYPES_H

#include "openpal/container/RSlice.h"
#include "openpal/container/WSlice.h"

#include "ssp21/crypto/gen/FormatError.h"
#include "ssp21/crypto/gen/ParseError.h"
#include "ssp21/crypto/Constants.h"

#include "ssp21/crypto/SequenceTypes.h"
#include "ssp21/crypto/IMessagePrinter.h"
#include "ssp21/crypto/IntegerField.h"

namespace ssp21
{

    class Seq8Seq16Field
    {

    public:

        Seq8Seq16Field();

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

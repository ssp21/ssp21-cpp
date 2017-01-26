
#ifndef SSP21_IFRAMEREADER_H
#define SSP21_IFRAMEREADER_H

#include "ssp21/SequenceTypes.h"

namespace ssp21
{
    /**
    * Abstract interface for reading the payload from some abstract frame type
    */
    class IFrameReader
    {
    public:

        virtual ~IFrameReader() {}

        // parse some input data, consuming some subset of it
        // if the return value is non-empty, a message was read from the stream
        virtual seq32_t read(seq32_t& input) = 0;

        // reset the state of the reader
        virtual void reset() = 0;

    };

    // A frame reader that treats input streams as the message
    class NullFrameReader final : public IFrameReader
    {

    public:

        virtual seq32_t read(seq32_t& input) override
        {
            return input;
        }

        // reset the state of the reader
        virtual void reset() override {}
    };
}

#endif

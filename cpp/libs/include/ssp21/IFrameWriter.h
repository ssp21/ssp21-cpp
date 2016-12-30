
#ifndef SSP21_IFRAMEWRITER_H
#define SSP21_IFRAMEWRITER_H

#include "ssp21/SequenceTypes.h"
#include "ssp21/crypto/FormatResult.h"

namespace ssp21
{
    /**
	*
    * Abstract interface for formatting a frame inside the cryptographic layer
	*
	* Allows crypto messages to be written directly into output message buffers to avoid
	* unnecessary copying.
	*
    */
    class IFrameWriter
    {

    public:        

        virtual FormatResult write(wseq32_t output) const = 0;

		virtual seq32_t get_frame() const = 0;

    };

}

#endif

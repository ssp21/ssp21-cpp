
#ifndef SSP21_IFRAMEWRITER_H
#define SSP21_IFRAMEWRITER_H

#include "ssp21/IWritable.h"

namespace ssp21
{

	class WriteResult final : public FormatResult
	{
	  public:

		  seq32_t frame;

		  static WriteResult error(FormatError err)
		  {
			  return WriteResult(err, seq32_t::empty(), seq32_t::empty());
		  }

		  static WriteResult success(FormatResult res, const seq32_t& frame)
		  {
			  return WriteResult(res.err, res.written, frame);
		  }

	  private:

		  WriteResult(FormatError err, const seq32_t& written, const seq32_t& frame) : FormatResult(err, written), frame(frame)
		  {}
	};

    /**
	*
    * Abstract interface for formatting a frame inside the cryptographic layer
	*
	* Allows crypto messages to be written directly into output message buffers to avoid
	* unnecessary copying.
	*
    */
    struct IFrameWriter
    {
		virtual ~IFrameWriter() {}

        virtual WriteResult write(IWritable& payload) = 0;		

		virtual uint16_t get_max_payload_size() const = 0;

    };

}

#endif

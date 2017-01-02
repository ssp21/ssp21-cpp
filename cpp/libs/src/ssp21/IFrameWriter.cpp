#include "ssp21/IFrameWriter.h"

#include "openpal/logging/LogMacros.h"

#include "ssp21/LogLevels.h"
#include "ssp21/crypto/LogMessagePrinter.h"

namespace ssp21
{

IFrameWriter::IFrameWriter(const openpal::Logger& logger) : logger(logger)
{}

        

WriteResult IFrameWriter::write(const IWritable& payload)
{
	const auto res = this->write_impl(payload);

	if (res.is_error())
	{
		FORMAT_LOG_BLOCK(this->logger, levels::error, "Error writing message: %s", FormatErrorSpec::to_string(res.err));
		return res;
	}	

	if (this->logger.is_enabled(levels::tx_crypto_msg_fields))
	{
		LogMessagePrinter printer(this->logger, levels::tx_crypto_msg_fields);
		payload.print(printer);
	}

	return res;
}

}


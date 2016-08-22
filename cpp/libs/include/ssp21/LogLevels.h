
#ifndef SSP21_LOGLEVELS_H
#define SSP21_LOGLEVELS_H

#include "openpal/logging/LogLevels.h"

namespace ssp21 
{
	const openpal::ModuleId moduleid = openpal::ModuleId(0xAAAAAAAA);

	namespace levels 
	{
		// mirror the openpal levels

		const openpal::LogLevel event = openpal::levels::event;
		const openpal::LogLevel error = openpal::levels::error;
		const openpal::LogLevel warn = openpal::levels::warn;
		const openpal::LogLevel info = openpal::levels::info;
		const openpal::LogLevel debug = openpal::levels::debug;

		// custom log levels
		const openpal::LogLevel rx_crypto_msg = debug.next();
		const openpal::LogLevel tx_crypto_msg = rx_crypto_msg.next();
		const openpal::LogLevel rx_crypto_msg_fields = tx_crypto_msg.next();
		const openpal::LogLevel tx_crypto_msg_fields = rx_crypto_msg_fields.next();

	}
}

#endif


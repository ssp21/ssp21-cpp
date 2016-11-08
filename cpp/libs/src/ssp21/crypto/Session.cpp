
#include "ssp21/crypto/Session.h"

#include "openpal/logging/LogMacros.h"

#include "ssp21/LogLevels.h"

namespace ssp21
{

Session::Session(const openpal::Logger& logger, uint16_t max_rx_payload_size) : 
	logger(logger),
	rx_auth_buffer(max_rx_payload_size)
{}
   
void Session::initialize(const Algorithms::Session& algorithms, const openpal::Timestamp& session_start, const SessionKeys& keys)
{
	this->valid = true;
    this->rx_nonce = this->tx_nonce = 0;
    this->algorithms = algorithms;
    this->session_start = session_start;
	this->keys.copy(keys);
}

void Session::reset()
{
	this->valid = false;
}

bool Session::validate(const UnconfirmedSessionData& message, const openpal::Timestamp& now)
{
	if (!this->valid) 
	{
		SIMPLE_LOG_BLOCK(this->logger, levels::warn, "ignoring message: no valid session");
		return false;
	}

	std::error_code ec;
	auto result = this->algorithms.read(this->keys.rx_key, message.metadata, message.payload, this->rx_auth_buffer.as_wslice(), ec);

	if (ec)
	{
		FORMAT_LOG_BLOCK(this->logger, levels::warn, "authentication failed: %s", ec.message().c_str());
		return false;
	}

	const auto current_session_time = now.milliseconds - this->session_start.milliseconds;

	// the message is authentic, check the TTL
	if (message.metadata.valid_until_ms > current_session_time)
	{
		SIMPLE_LOG_BLOCK(this->logger, levels::warn, "ignoring message: TTL no longer valid");
		return false;
	}

	// check the nonce
	if (!this->algorithms.verify_nonce(this->rx_nonce, message.metadata.nonce.value))
	{
		SIMPLE_LOG_BLOCK(this->logger, levels::warn, "ignoring message: bad nonce");
		return false;
	}

	this->rx_nonce = message.metadata.nonce.value;

	return true;
}

}



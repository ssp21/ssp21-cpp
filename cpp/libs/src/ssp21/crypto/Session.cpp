
#include "ssp21/crypto/Session.h"

namespace ssp21
{
   
void Session::initialize(const Algorithms::Session& algorithms, const openpal::Timestamp& session_start, const SessionKeys& keys)
{
	this->valid = true;
    this->rx_nonce = this->tx_nonce = 0;
    this->algorithms = algorithms;
    this->session_start = session_start;
	this->keys.copy(keys);
}

void Session::close()
{
	this->valid = false;
}

}



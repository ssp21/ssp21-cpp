
#ifndef SSP21_STATISTICS_H
#define SSP21_STATISTICS_H

#include <cstdint>

namespace ssp21
{
	struct Statistic
	{
		uint64_t value = 0;

		inline void increment()
		{
			++value;
		}
		
		operator const uint64_t& () const { return value; }
	};

	struct SessionStatistics
	{
		Statistic num_init;
		Statistic num_reset;		
		Statistic num_user_data_without_session;
		Statistic num_auth_fail;
		Statistic num_ttl_expiration;
		Statistic num_nonce_fail;
		Statistic num_success;
	};

	struct ResponderStatistics
	{
		ResponderStatistics(const SessionStatistics& session) : 
			session(session) 
		{}

		SessionStatistics session;
	};
}

#endif


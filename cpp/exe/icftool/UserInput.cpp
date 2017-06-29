#include "UserInput.h"

#include <iostream>
#include <chrono>

using namespace std::chrono;

Actions::Times UserInput::get_validity_times_from_user()
{
	std::cout << "How many days (from now) should the certificate remain valid?" << std::endl;
	uint16_t days = 0;
	std::cin >> days;
	return days_to_times(days);
}

uint8_t UserInput::get_signing_level_from_user()
{
	std::cout << "Enter the certificate signing level (0 == endpoint certificate, max == 6):" << std::endl;
	uint32_t signing_level = 256;

	while (signing_level > 6)
	{
		std::cin >> signing_level;
	}

	return static_cast<uint8_t>(signing_level);
}

Actions::Times UserInput::days_to_times(uint16_t days)
{
	const uint64_t valid_after_ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	const auto valid_before_ms = valid_after_ms + (days * 86400 * 1000);

	return Actions::Times{ valid_after_ms, valid_before_ms };
}

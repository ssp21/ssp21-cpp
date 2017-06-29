
#ifndef ICFTOOL_USERINPUT_H
#define ICFTOOL_USERINPUT_H

#include "Actions.h"

class UserInput
{

public:

	static Actions::Times get_validity_times_from_user();
	static uint8_t get_signing_level_from_user();   
   

	static Actions::Times days_to_times(uint16_t days);	
};

#endif

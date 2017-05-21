
#include "ssp21/crypto/Crypto.h"

#include <iostream>

using namespace ssp21;

int main(int argc, char*  argv[])
{
	if (!ssp21::Crypto::initialize()) {
		std::cerr << "Unable to initialize crypto backend" << std::endl;
		return -1;
	}

    KeyPair kp;
    Crypto::gen_keypair_x25519(kp);

	/*
	TODO - update key writing
	*/

    return 0;
}

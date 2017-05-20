
#include "ssp21/crypto/Crypto.h"
#include "ssp21/util/Base64.h"

#include <iostream>

using namespace ssp21;

int main(int argc, char*  argv[])
{
    if (!ssp21::Crypto::initialize()) return -1;

    KeyPair kp;
    Crypto::gen_keypair_x25519(kp);

    std::cout << "public key:" << std::endl;
    Base64::encode(kp.public_key.as_seq(), [](char c)
    {
        std::cout << c;
    });
    std::cout << std::endl;

    std::cout << "private key:" << std::endl;
    Base64::encode(kp.private_key.as_seq(), [](char c)
    {
        std::cout << c;
    });
    std::cout << std::endl;

    return 0;
}


#include "ssp21/crypto/Crypto.h"

#include "ssp21/crypto/gen/CertificateFile.h"
#include "ssp21/util/SecureFile.h"

#include <iostream>
#include <fstream>

using namespace ssp21;

bool write(const std::string& path, FileEntryType type, const seq16_t& data);

int main(int argc, char*  argv[])
{
    if (!ssp21::Crypto::initialize())
    {
        std::cerr << "Unable to initialize crypto backend" << std::endl;
        return -1;
    }

    KeyPair kp;
    Crypto::gen_keypair_x25519(kp);

    if (!write("public_key.icf", FileEntryType::x25519_public_key, kp.public_key.as_seq().widen<uint16_t>()))
    {
        std::cerr << "Unable to write public key" << std::endl;
        return -1;
    }

    if (!write("private_key.icf", FileEntryType::x25519_private_key, kp.private_key.as_seq().widen<uint16_t>()))
    {
        std::cerr << "Unable to write private key" << std::endl;
        return -1;
    }

    return 0;
}

bool write(const std::string& path, FileEntryType type, const seq16_t& data)
{
    CertificateFile file;

    file.entries.push(
        CertificateFileEntry(
            FileEntryType::x25519_private_key,
            data
        )
    );

    SecureFile output;

    return output.write(path, [&](wseq32_t& buffer)
    {
        return file.write(buffer);
    });
}
#ifndef SSP21_MOCKCERTIFICATEDATA_H
#define SSP21_MOCKCERTIFICATEDATA_H

#include "ser4cpp/container/Buffer.h"
#include "ser4cpp/util/Uncopyable.h"

#include "ssp21/crypto/gen/CertificateEnvelope.h"
#include "ssp21/crypto/gen/CertificateBody.h"

#include <memory>

namespace ssp21
{
    /*
    	An envelope and certificate body backed by an owned buffer
    */
    class MockCertificateData : private ser4cpp::Uncopyable
    {
        const std::unique_ptr<ser4cpp::Buffer> public_key_data;
        const std::unique_ptr<ser4cpp::Buffer> signature_data;

    public:
        const CertificateBody body;

    private:

        const std::unique_ptr<ser4cpp::Buffer> body_data;

    public:

        const CertificateEnvelope envelope;

        MockCertificateData(uint8_t signing_level, PublicKeyType public_key_type);

    private:

        static uint8_t get_size(PublicKeyType type);
        static std::unique_ptr<ser4cpp::Buffer> allocate(uint8_t size);

        MockCertificateData() = delete;
    };

}

#endif
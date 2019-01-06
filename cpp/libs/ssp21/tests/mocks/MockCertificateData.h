#ifndef SSP21_MOCKCERTIFICATEDATA_H
#define SSP21_MOCKCERTIFICATEDATA_H

#include "openpal/container/Buffer.h"

#include "crypto/gen/CertificateEnvelope.h"
#include "crypto/gen/CertificateBody.h"

#include "openpal/util/Uncopyable.h"

#include <memory>

namespace ssp21
{
    /*
    	An envelope and certificate body backed by an owned buffer
    */
    class MockCertificateData : private openpal::Uncopyable
    {
        const std::unique_ptr<openpal::Buffer> public_key_data;
        const std::unique_ptr<openpal::Buffer> signature_data;

    public:
        const CertificateBody body;

    private:

        const std::unique_ptr<openpal::Buffer> body_data;

    public:

        const CertificateEnvelope envelope;

        MockCertificateData(uint8_t signing_level, PublicKeyType public_key_type);

    private:

        static uint8_t get_size(PublicKeyType type);
        static std::unique_ptr<openpal::Buffer> allocate(uint8_t size);

        MockCertificateData() = delete;
    };

}

#endif
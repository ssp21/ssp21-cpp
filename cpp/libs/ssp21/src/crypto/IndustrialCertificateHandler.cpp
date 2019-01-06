

#include "crypto/IndustrialCertificateHandler.h"

#include "crypto/gen/CertificateChain.h"
#include "crypto/gen/ContainerFile.h"

#include "ssp21/crypto/Chain.h"

#include "ssp21/util/Exception.h"

namespace ssp21
{

    IndustrialCertificateHandler::IndustrialCertificateHandler(
        const std::shared_ptr<ssp21::SecureDynamicBuffer>& anchor_cert_file_data,
        const std::shared_ptr<ssp21::SecureDynamicBuffer>& presented_chain_file_data
    ) :
        anchor_certificate_file_data(anchor_cert_file_data),
        presented_chain_file_data(presented_chain_file_data),
        anchor_certificate_body(read_anchor_cert(anchor_certificate_file_data->as_rslice())),
        presented_certificate_data(verify_presented_chain(anchor_certificate_body, presented_chain_file_data->as_rslice()))
    {

    }

    CertificateBody IndustrialCertificateHandler::read_anchor_cert(const seq32_t& file_data)
    {
        ContainerFile file;
        {
            const auto err = file.read_all(file_data);
            if (any(err)) throw Exception("Unable to read container file: ", ParseErrorSpec::to_string(err));
        }

        if (file.container_entry_type != ContainerEntryType::certificate_chain)
        {
            throw Exception("Unexpected file type: ", ContainerEntryTypeSpec::to_string(file.container_entry_type));
        }

        CertificateChain chain;
        {
            const auto err = chain.read_all(file.payload);
            if (any(err)) throw Exception("Unable to read certificate chain: ", ParseErrorSpec::to_string(err));
        }

        if (chain.certificates.count() != 1) throw Exception("Unexpected number of certificates: ", chain.certificates.count());

        CertificateBody body;
        const auto err = body.read_all(chain.certificates.get(0)->certificate_body);
        if (any(err)) throw Exception("Unable to read certificate body: ", ParseErrorSpec::to_string(err));

        return body;
    }

    seq32_t IndustrialCertificateHandler::verify_presented_chain(const CertificateBody& anchor, const seq32_t& file_data)
    {
        ContainerFile file;
        {
            const auto err = file.read_all(file_data);
            if (any(err)) throw Exception("Unable to read container file: ", ParseErrorSpec::to_string(err));
        }

        if (file.container_entry_type != ContainerEntryType::certificate_chain)
        {
            throw Exception("Unexpected file type: ", ContainerEntryTypeSpec::to_string(file.container_entry_type));
        }

        CertificateChain chain;
        {
            const auto err = chain.read_all(file.payload);
            if (any(err)) throw Exception("Unable to read certificate chain: ", ParseErrorSpec::to_string(err));
        }

        {
            // verify the chain
            CertificateBody endpoint;
            const auto err = Chain::verify(anchor, chain.certificates, endpoint);
            if (any(err)) throw Exception("Error verifying certificate chain: ", HandshakeErrorSpec::to_string(err));
        }

        return file.payload;
    }

    seq32_t IndustrialCertificateHandler::certificate_data() const
    {
        return this->presented_certificate_data;
    }

    HandshakeError IndustrialCertificateHandler::validate(HandshakeMode mode, const seq32_t& certificate_data, seq32_t& public_key_output)
    {
        if (mode != HandshakeMode::industrial_certificates) return HandshakeError::unsupported_handshake_mode;

        // first parse the data as a certificate chain
        CertificateChain chain;
        {
            const auto err = chain.read_all(certificate_data);
            if (any(err)) return HandshakeError::bad_certificate_format;
        }

        CertificateBody endpoint_cert;
        const auto err = Chain::verify(this->anchor_certificate_body, chain.certificates, endpoint_cert);
        if (any(err)) return err;

        public_key_output = endpoint_cert.public_key;

        return HandshakeError::none;
    }

}



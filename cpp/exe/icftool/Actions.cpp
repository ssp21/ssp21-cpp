
#include "Actions.h"

#include "ssp21/crypto/Crypto.h"
#include "ssp21/crypto/Chain.h"

#include "ssp21/crypto/gen/CertificateBody.h"
#include "ssp21/crypto/gen/CertificateEnvelope.h"
#include "ssp21/crypto/gen/ContainerFile.h"

#include "ssp21/util/SecureFile.h"

#include <chrono>

using namespace std::chrono;
using namespace ssp21;


template <class T>
static T parse_or_throw(const ssp21::seq32_t& data)
{
    T item;
    const auto err = item.read_all(data);
    if (any(err))
    {
        throw ssp21::Exception("Parsing error: ", ssp21::ParseErrorSpec::to_string(err));
    }
    return item;
}

void Actions::print_contents(const std::string& path)
{
    const auto data = SecureFile::read(path);

    ContainerFile file;
    const auto err = file.read_all(data->as_rslice());
    if (any(err))
    {
        throw Exception("Error parsing container file: ", ParseErrorSpec::to_string(err));
    }

    std::cout << "File: " << path << std::endl;
    std::cout << "Type: " << ContainerEntryTypeSpec::to_string(file.container_entry_type) << std::endl;

    ConsolePrinter printer;

    if (file.container_entry_type == ContainerEntryType::certificate_chain)
    {
        print_certificate_chain(printer, file.payload);
    }
    else
    {
        // all other types are raw keys
        printer.push_indent();
        printer.print("key-data", file.payload);
    }

    std::cout << std::endl;
}

void Actions::gen_x25519_key_pair(const std::string& private_key_path, const std::string& public_key_path)
{
    std::cout << "generating x25519 key pair..." << std::endl;

    KeyPair kp;
    Crypto::gen_keypair_x25519(kp);

    write(private_key_path, ContainerEntryType::x25519_private_key, kp.private_key.as_seq());

    write(public_key_path, ContainerEntryType::x25519_public_key, kp.public_key.as_seq());
}

void Actions::gen_ed25519_key_pair(const std::string& private_key_path, const std::string& public_key_path)
{
    std::cout << "generating Ed25519 key pair" << std::endl;

    KeyPair kp;
    Crypto::gen_keypair_ed25519(kp);

    write(private_key_path, ContainerEntryType::ed25519_private_key, kp.private_key.as_seq());

    write(public_key_path, ContainerEntryType::ed25519_public_key, kp.public_key.as_seq());
}

void Actions::append_certificate_chains(const std::string& file_path_1, const std::string& file_path_2, const std::string& output_file_path)
{

    const auto file_data_1 = SecureFile::read(file_path_1);
    const auto chain_1 = expect_certificate_chain(parse_or_throw<ContainerFile>(file_data_1->as_rslice()));

    const auto file_data_2 = SecureFile::read(file_path_2);
    const auto chain_2 = expect_certificate_chain(parse_or_throw<ContainerFile>(file_data_2->as_rslice()));

	if (chain_1.certificates.is_empty())
	{
		throw Exception("The initial chain is empty. Must contain 1 or more certificates.");
	}
	
	
	if (chain_2.certificates.count() != 1)
	{
		throw Exception("The certificate chain being appended doesn't contain a single entry, it contains: ", chain_2.certificates.count());
	}

	// extract the body of the last certificate in the initial chain
	CertificateBody last_body = parse_or_throw<CertificateBody>(chain_1.certificates.last()->certificate_body);
	
	
	
	{
		// Verify that the certificated being appended can be verified using the last certificate in the existing chain
		CertificateBody final_body;
		const auto err = Chain::verify(last_body, chain_2.certificates, final_body);
		if (any(err))
		{
			throw Exception("Error verifying certificate chain: ", HandshakeErrorSpec::to_string(err));
		}			
	}

    CertificateChain chain;

    auto add_cert = [&chain](const CertificateEnvelope & envelope)
    {
        if (!chain.certificates.push(envelope))
        {
            throw Exception("Number of total certificates entries exceeds capacity limit of ", chain.certificates.capacity());
        }
    };

    chain_1.certificates.foreach(add_cert);
    chain_2.certificates.foreach(add_cert);

    const auto chain_bytes = serialize::to_buffer(chain);

    SecureFile::write(
        output_file_path,
        ContainerFile(ContainerEntryType::certificate_chain, chain_bytes->as_rslice())
    );
}

void Actions::create_certificate(const std::string& certificate_file_path, const std::string& public_key_path, const std::string& private_key_path)
{
    const auto private_key_data = SecureFile::read(private_key_path);
    const auto private_key_entry = parse_or_throw<ContainerFile>(private_key_data->as_rslice());

    const auto public_key_data = SecureFile::read(public_key_path);
    const auto public_key_entry = parse_or_throw<ContainerFile>(public_key_data->as_rslice());

    // interactively read stuff from the user
    const auto validity = get_validity_times_from_user();
    const auto signing_level = get_signing_level_from_user();

    const CertificateBody body(
        validity.valid_after,
        validity.valid_before,
        signing_level,
        get_public_key_type(public_key_entry),
        public_key_entry.payload
    );

    const auto body_bytes = serialize::to_buffer(body);

    DSAOutput signature;
    calc_signature(body_bytes->as_rslice(), private_key_entry, signature);

    const CertificateEnvelope envelope(
        signature.as_seq(),
        body_bytes->as_rslice().take(static_cast<uint16_t>(body_bytes->length()))
    );

    CertificateChain chain;
    chain.certificates.push(envelope);

    const auto chain_bytes = serialize::to_buffer(chain);

    const ContainerFile file(ContainerEntryType::certificate_chain, chain_bytes->as_rslice());

    SecureFile::write(certificate_file_path, file);
}

void Actions::calc_signature(const seq32_t& data, const ContainerFile& private_key_entry, DSAOutput& signature)
{
    std::error_code ec;

    switch (private_key_entry.container_entry_type)
    {
    case(ContainerEntryType::ed25519_private_key):
        Crypto::sign_ed25519(data, private_key_entry.payload, signature, ec);
        if (ec) throw Exception("Unable to calculate certificate siganture", ec.message());
        break;
    default:
        throw Exception(ContainerEntryTypeSpec::to_string(private_key_entry.container_entry_type), " is not a private signing key");
    }
}

PublicKeyType Actions::get_public_key_type(const ContainerFile& file)
{
    switch (file.container_entry_type)
    {
    case(ContainerEntryType::ed25519_public_key):
        if (file.payload.length() != consts::crypto::ed25519_public_key_length) throw Exception("Bad Ed25519 public key length");
        return PublicKeyType::Ed25519;
    case(ContainerEntryType::x25519_public_key):
        if (file.payload.length() != consts::crypto::x25519_key_length) throw Exception("Bad x25519 public key length");
        return PublicKeyType::X25519;
    default:
        throw Exception(ContainerEntryTypeSpec::to_string(file.container_entry_type), " is not a public key");
    }
}

ssp21::CertificateChain Actions::expect_certificate_chain(const ssp21::ContainerFile& file)
{
    if (file.container_entry_type != ContainerEntryType::certificate_chain)
    {
        throw Exception(ContainerEntryTypeSpec::to_string(file.container_entry_type), " is not a certificate chain");
    }

    return parse_or_throw<ssp21::CertificateChain>(file.payload);
}

void Actions::write(const std::string& path, ContainerEntryType type, const seq32_t& data)
{
    SecureFile::write(path, ContainerFile(type, data));
    std::cout << "wrote: " << path << std::endl;
}

void Actions::print_certificate_chain(ConsolePrinter& printer, const seq32_t& data)
{
    const auto chain = parse_or_throw<CertificateChain>(data);

    int i = 1;

    auto print = [&](const CertificateEnvelope & envelope)
    {
        std::cout << std::endl;
        std::cout << "certificate #" << i << std::endl;
        printer.push_indent();
        envelope.print("envelope", printer);

        CertificateBody body;
        const auto body_err = body.read_all(envelope.certificate_body);
        if (any(body_err))
        {
            throw Exception("Error parsing certificate body: ", ParseErrorSpec::to_string(body_err));
        }

        printer.push_indent();
        printer.push_indent();
        body.print("body", printer);
        printer.pop_indent();
        printer.pop_indent();
        ++i;
    };

    chain.certificates.foreach(print);
}

Actions::Times Actions::get_validity_times_from_user()
{
    std::cout << "How many days (from now) should the certificate remain valid?" << std::endl;
    uint16_t days = 0;
    std::cin >> days;

    const uint64_t valid_after_ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    const auto valid_before_ms = valid_after_ms + (days * 86400 * 1000);

    return Times{ valid_after_ms, valid_before_ms };
}

uint8_t Actions::get_signing_level_from_user()
{
    std::cout << "Enter the certificate signing level (0 == endpoint certificate, max == 6):" << std::endl;
    uint32_t signing_level = 256;

    while (signing_level > 6)
    {
        std::cin >> signing_level;
    }

    return static_cast<uint8_t>(signing_level);
}

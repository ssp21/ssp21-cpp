
#include "Actions.h"

#include "ssp21/crypto/Crypto.h"

#include "ssp21/crypto/gen/CertificateBody.h"
#include "ssp21/crypto/gen/CertificateEnvelope.h"
#include "ssp21/crypto/gen/ContainerFile.h"

#include "ssp21/util/SecureFile.h"

#include "ConsolePrinter.h"

#include <chrono>

using namespace std::chrono;
using namespace ssp21;

template <class T>
void read_or_throw(T& item, const ssp21::seq32_t& data, const std::string& path)
{
    const auto err = item.read_all(data);
    if (any(err))
    {
        throw ssp21::Exception("Encountered error: ", ssp21::ParseErrorSpec::to_string(err), " parsing input file: ", path);
    }
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

    std::cout << path << " contains " << file.entries.count() << " item(s):" << std::endl << std::endl;

    ConsolePrinter printer;

    for (auto i = 0; i < file.entries.count(); ++i)
    {
        const auto entry = file.entries.get(i);
        const auto type_name = FileEntryTypeSpec::to_string(entry->file_entry_type);

        std::cout << "item #" << i << " is a " << type_name << std::endl;

        printer.push_indent();

        if (entry->file_entry_type == FileEntryType::certificate)
        {
            print_certificate(printer, entry->data);
        }
        else
        {
            // all other types are raw keys
            printer.print("key-data", entry->data);
        }

        std::cout << std::endl;
    }
}

void Actions::gen_x25519_key_pair(const std::string& private_key_path, const std::string& public_key_path)
{
    std::cout << "generating x25519 key pair" << std::endl;

    KeyPair kp;
    Crypto::gen_keypair_x25519(kp);

    write(private_key_path, FileEntryType::x25519_private_key, kp.private_key.as_seq());

    write(public_key_path, FileEntryType::x25519_public_key, kp.public_key.as_seq());
}

void Actions::gen_ed25519_key_pair(const std::string& private_key_path, const std::string& public_key_path)
{
    std::cout << "generating Ed25519 key pair" << std::endl;

    KeyPair kp;
    Crypto::gen_keypair_ed25519(kp);

    write(private_key_path, FileEntryType::ed25519_private_key, kp.private_key.as_seq());

    write(public_key_path, FileEntryType::ed25519_public_key, kp.public_key.as_seq());
}

void Actions::append(const std::string& file_path_1, const std::string& file_path_2, const std::string& output_file_path)
{
    const auto file_data_1 = SecureFile::read(file_path_1);
    ContainerFile file_1;
    read_or_throw(file_1, file_data_1->as_rslice(), file_path_1);

    const auto file_data_2 = SecureFile::read(file_path_2);
    ContainerFile file_2;
    read_or_throw(file_2, file_data_2->as_rslice(), file_path_2);

    ContainerFile output;
    auto add_entry = [&output](const CertificateFileEntry & entry)
    {
        if (!output.entries.push(entry))
        {
            throw Exception("Number of total file entries exceeds capacity limit of ", output.entries.capacity());
        }
    };

    file_1.entries.foreach(add_entry);
    file_2.entries.foreach(add_entry);

    SecureFile::write(output_file_path, output);
}

void Actions::create_certificate(const std::string& certificate_file_path, const std::string& public_key_path, const std::string& private_key_path)
{
    const auto private_key_data = SecureFile::read(private_key_path);
    const auto private_key_entry = get_only_entry(private_key_data->as_rslice());

    const auto public_key_data = SecureFile::read(public_key_path);
    const auto public_key_entry = get_only_entry(public_key_data->as_rslice());

    // interactively read stuff from the user
    const auto validity = get_validity_times_from_user();
    const auto signing_level = get_signing_level_from_user();

    CertificateBody body(
        validity.valid_after,
        validity.valid_before,
        signing_level,
        get_public_key_type(public_key_entry),
        public_key_entry.data.take(consts::crypto::x25519_key_length)
    );

    const auto body_bytes = serialize::to_buffer(body);

    DSAOutput signature;
    calc_signature(body_bytes->as_rslice(), private_key_entry, signature);

    const auto envelope_bytes = serialize::to_buffer(
                                    CertificateEnvelope(
                                        seq32_t::empty(),
                                        signature.as_seq(),
                                        body_bytes->as_rslice().take(static_cast<uint16_t>(body_bytes->length()))
                                    )
                                );

    ContainerFile file;
    file.entries.push(
        CertificateFileEntry(
            FileEntryType::certificate,
            envelope_bytes->as_rslice()
        )
    );

    SecureFile::write(certificate_file_path, file);
}

void Actions::calc_signature(const seq32_t& data, const CertificateFileEntry& private_key_entry, DSAOutput& signature)
{
    std::error_code ec;

    switch (private_key_entry.file_entry_type)
    {
    case(FileEntryType::ed25519_private_key):
        Crypto::sign_ed25519(data, private_key_entry.data, signature, ec);
        if (ec) throw Exception("Unable to calculate certificate siganture", ec.message());
        break;
    default:
        throw Exception("Can't produce a signature from: ", FileEntryTypeSpec::to_string(private_key_entry.file_entry_type));
    }
}

PublicKeyType Actions::get_public_key_type(const CertificateFileEntry& entry)
{
    switch (entry.file_entry_type)
    {
    case(FileEntryType::ed25519_public_key):
        if (entry.data.length() != consts::crypto::ed25519_public_key_length) throw Exception("Bad Ed25519 public key length");
        return PublicKeyType::Ed25519;
    case(FileEntryType::x25519_public_key):
        if (entry.data.length() != consts::crypto::x25519_key_length) throw Exception("Bad x25519 public key length");
        return PublicKeyType::X25519;
    default:
        throw Exception(FileEntryTypeSpec::to_string(entry.file_entry_type), " is not a public key");
    }
}

CertificateFileEntry Actions::get_only_entry(const seq32_t& file_data)
{
    ContainerFile file;
    const auto err = file.read_all(file_data);
    if (any(err))
    {
        throw Exception("Encountered error: ", ParseErrorSpec::to_string(err), " parsing key file");
    }

    if (file.entries.count() != 1)
    {
        throw Exception("Key file contains doesn't contains a single entry, contains: ", file.entries.count());
    }

    return *file.entries.get(0);
}

void Actions::write(const std::string& path, FileEntryType type, const seq32_t& data)
{
    ContainerFile file;
    file.entries.push(CertificateFileEntry(type, data));
    SecureFile::write(path, file);
}

void Actions::print_certificate(IMessagePrinter& printer, const seq32_t& data)
{
    CertificateEnvelope envelope;

    const auto err = envelope.read_all(data);
    if (any(err))
    {
        throw Exception("Error parsing certificate envelope: ", ParseErrorSpec::to_string(err));
    }

    envelope.print("envelope", printer);

    CertificateBody body;
    const auto body_err = body.read_all(envelope.certificate_body);
    if (any(body_err))
    {
        throw Exception("Error parsing certificate body: ", ParseErrorSpec::to_string(body_err));
    }

    body.print("body", printer);
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

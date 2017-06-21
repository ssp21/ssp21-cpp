
#ifndef ICFTOOL_ACTIONS_H
#define ICFTOOL_ACTIONS_H

#include "ssp21/crypto/IMessagePrinter.h"
#include "ssp21/crypto/BufferTypes.h"

#include "ssp21/crypto/gen/CertificateFileEntry.h"
#include "ssp21/crypto/gen/PublicKeyType.h"

#include "ssp21/util/SequenceTypes.h"
#include "ssp21/util/Exception.h"

#include <string>

class Actions
{

public:

    static void print_contents(const std::string& path);
    static void gen_x25519_key_pair(const std::string& private_key_path, const std::string& public_key_path);
    static void gen_ed25519_key_pair(const std::string& private_key_path, const std::string& public_key_path);
    static void append(const std::string& file1, const std::string& file2, const std::string& output_file);
    static void create_certificate(const std::string& certificate_file, const std::string& private_key_path, const std::string& public_key_path);

private:

    static void print_certificate(ssp21::IMessagePrinter& printer, const ssp21::seq32_t& data);
    static void calc_signature(const ssp21::seq32_t& data, const ssp21::CertificateFileEntry& private_key_entry, ssp21::DSAOutput& signature);
    static ssp21::PublicKeyType get_public_key_type(const ssp21::CertificateFileEntry& entry);
    static ssp21::CertificateFileEntry get_only_entry(const ssp21::seq32_t& data);
    static void write(const std::string& path, ssp21::FileEntryType type, const ssp21::seq32_t& data);

    // ----- getting inputs from the user -----

    struct Times
    {
        uint64_t valid_before;
        uint64_t valid_after;
    };

    static Times get_validity_times_from_user();

	static uint8_t get_signing_level_from_user();
};

#endif

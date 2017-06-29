
#ifndef ICFTOOL_ACTIONS_H
#define ICFTOOL_ACTIONS_H

#include "ssp21/crypto/IMessagePrinter.h"
#include "ssp21/crypto/BufferTypes.h"

#include "ssp21/crypto/gen/ContainerFile.h"
#include "ssp21/crypto/gen/PublicKeyType.h"
#include "ssp21/crypto/gen/CertificateChain.h"

#include "ssp21/util/SequenceTypes.h"
#include "ssp21/util/Exception.h"

#include "ConsolePrinter.h"
#include <string>


class Actions
{
	
public:	

	struct Times
	{
		uint64_t valid_before;
		uint64_t valid_after;
	};

    static void print_contents(const std::string& path);
    static void gen_x25519_key_pair(const std::string& private_key_path, const std::string& public_key_path);
    static void gen_ed25519_key_pair(const std::string& private_key_path, const std::string& public_key_path);
    static void append_certificate_chains(const std::string& file1, const std::string& file2, const std::string& output_file);
    
	static void create_certificate(
		const std::string& certificate_file,
		const std::string& private_key_path, 
		const std::string& public_key_path,
		const Times& validity_times,
		const uint8_t signing_level
	);

private:

    static void print_certificate_chain(ConsolePrinter& printer, const ssp21::seq32_t& data);
    static void calc_signature(const ssp21::seq32_t& data, const ssp21::ContainerFile& private_key_entry, ssp21::DSAOutput& signature);

    static ssp21::PublicKeyType get_public_key_type(const ssp21::ContainerFile& file);
    static ssp21::CertificateChain expect_certificate_chain(const ssp21::ContainerFile& data);
    static void write(const std::string& path, ssp21::ContainerEntryType type, const ssp21::seq32_t& data);
    
};

#endif

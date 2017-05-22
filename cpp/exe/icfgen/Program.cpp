
#include "Program.h"

#include "ssp21/crypto/Crypto.h"

#include "ssp21/crypto/gen/CertificateFile.h"
#include "ssp21/util/SecureFile.h"

#include <iostream>

using namespace ssp21;

void gen_x25519_key_pair(const std::string& private_key_path, const std::string& public_key_path);
void write(const std::string& path, FileEntryType type, const seq16_t& data);


Program::Program() :
	parser{{
		{ flags::help,{ "-h", "--help" }, "shows this help message", 0 },
		{ flags::x25519,{ "-x", "--x25519" }, "generate an x25519 key pair <private key file> <public key file>", 0 },
	}}
{
	if (!ssp21::Crypto::initialize())
	{
		throw std::exception("unable to initialize crypto backend");
	}
}

void Program::run(int argc, char*  argv[])
{	
	const auto args = this->parser.parse(argc, argv);	

	if (args[flags::help])
	{
		this->print_help();
		return;
	}

	if (args.has_option(flags::x25519))
	{		
		const auto files = args[flags::x25519];

		if (args.pos.size() != 2)
		{
			throw std::exception("You must specify the <private key file> and the <public key file>");
		}
			

		gen_x25519_key_pair(args.pos[0], args.pos[1]);
		return;
	}
		
	std::cerr << "You must specify an option" << std::endl << std::endl;
	this->print_help();	
}

void Program::print_help()
{
	std::cerr << "Options: " << std::endl << std::endl;
	std::cerr << this->parser;
}

void gen_x25519_key_pair(const std::string& private_key_path, const std::string& public_key_path)
{
	KeyPair kp;
	Crypto::gen_keypair_x25519(kp);

	write(private_key_path, FileEntryType::x25519_private_key, kp.private_key.as_seq().widen<uint16_t>());

	write(public_key_path, FileEntryType::x25519_public_key, kp.public_key.as_seq().widen<uint16_t>());
}

void write(const std::string& path, FileEntryType type, const seq16_t& data)
{
	CertificateFile file;

	file.entries.push(
		CertificateFileEntry(
			FileEntryType::x25519_private_key,
			data
		)
	);

	SecureFile output;

	auto handler = [&](wseq32_t& buffer) { return file.write(buffer); };

	if (!output.write(path, handler))
	{
		std::ostringstream oss;
		oss << "Unable to write file: " << path;
		throw std::logic_error(oss.str());
	}
}


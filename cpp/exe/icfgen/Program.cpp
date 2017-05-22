
#include "Program.h"

#include "ssp21/crypto/Crypto.h"

#include "ssp21/crypto/gen/CertificateFile.h"
#include "ssp21/crypto/gen/CertificateEnvelope.h"
#include "ssp21/util/SecureFile.h"

#include <iostream>

#include "ssp21/crypto/LogMessagePrinter.h"

#include "ConsolePrinter.h"

using namespace ssp21;

void print_contents(const std::string& path);
void print_certificate(IMessagePrinter& printer, const seq32_t& data);
void gen_x25519_key_pair(const std::string& private_key_path, const std::string& public_key_path);
void write(const std::string& path, FileEntryType type, const seq16_t& data);

Program::Program() :
	parser{{
		{ flags::help, { "-h", "--help" }, "shows this help message", 0 },
		{ flags::x25519, { "-x", "--x25519" }, "generate an x25519 key pair <private key file> <public key file>", 0 },
		{ flags::show, { "-s", "--show" }, "show contents of an <icf file>", 1 },
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

	if (args.has_option(flags::show))
	{	
		print_contents(args[flags::show]);		
		return;
	}

	if (args.has_option(flags::x25519))
	{		
		const auto files = args[flags::x25519];

		if (args.pos.size() != 2)
		{
			throw std::exception("Required positional arguments: <private key file> <public key file>");
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

	file.entries.push(CertificateFileEntry(type, data));

	SecureFile output;

	auto handler = [&](wseq32_t& buffer) { return file.write(buffer); };

	if (!output.write(path, handler))
	{
		std::ostringstream oss;
		oss << "Unable to write file: " << path;
		throw std::logic_error(oss.str());
	}
}

void print_contents(const std::string& path)
{
	SecureFile output;
	const auto data = output.read(path);
	if (data.is_empty())
	{
		std::ostringstream oss;
		oss << "Unable to read file: " << path;
		throw std::logic_error(oss.str());
	}

	CertificateFile file;
	const auto err = file.read_all(data);
	if (any(err))
	{
		std::ostringstream oss;
		oss << "Error parsing certificate file: " << ParseErrorSpec::to_string(err);
		throw std::logic_error(oss.str());
	}	

	std::cout << path << " contains " << file.entries.count() << " item(s):" << std::endl << std::endl;

	ConsolePrinter printer;

	for (auto i = 0; i < file.entries.count(); ++i)
	{
		const auto entry = file.entries.get(i);
		const auto type_name = FileEntryTypeSpec::to_string(entry->file_entry_type);

		std::cout << "item #" << (i + 1) << " is a " << type_name << std::endl;

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

void print_certificate(IMessagePrinter& printer, const seq32_t& data)
{
	CertificateEnvelope envelope;

	const auto err = envelope.read_all(data);



}
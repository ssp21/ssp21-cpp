
#include "Program.h"

#include "Actions.h"

#include "sodium/SodiumBackend.h"

#include <iostream>

using namespace ssp21;

Program::Program() :
    parser{{
        { flags::help, { "-h", "--help" }, "shows this help message", 0 },
        { flags::show, { "-s", "--show" }, "show contents of one or more <icf files ....>", 0 },
        { flags::x25519, { "-x", "--x25519" }, "generate a x25519 key pair <private key file> <public key file>", 0 },
        { flags::ed25519, { "-d", "--ed25519" }, "generate a Ed25519 key pair <private key file> <public key file>", 0 },
        { flags::cert, { "-c", "--cert" }, "interactively generate a <certificate file> for a <public key file> signed by a <private key file>", 0 },
        { flags::cert, { "-a", "--append" }, "append all entries from one <icf file> and another <icf file> and write them to a destination <icf file>", 0 }
    }}
{
    if (!ssp21::SodiumBackend::initialize())
    {
        throw ssp21::Exception("unable to initialize crypto backend");
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
        for (auto& file_path : args.pos)
        {
            Actions::print_contents(file_path);
        }

        return;
    }

    if (args.has_option(flags::x25519))
    {
        if (args.pos.size() != 2)
        {
            throw ssp21::Exception("Required positional arguments: <private key file> <public key file>");
        }

        Actions::gen_x25519_key_pair(args.pos[0], args.pos[1]);
        return;
    }

    if (args.has_option(flags::ed25519))
    {
        if (args.pos.size() != 2)
        {
            throw ssp21::Exception("Required positional arguments: <private key file> <public key file>");
        }

        Actions::gen_ed25519_key_pair(args.pos[0], args.pos[1]);
        return;
    }

    if (args.has_option(flags::cert))
    {
        if (args.pos.size() != 3)
        {
            throw ssp21::Exception("Required positional arguments: <certificate file> <public key file> <private key file>");
        }

        Actions::create_certificate(args.pos[0], args.pos[1], args.pos[2]);
        return;
    }

    if (args.has_option(flags::append))
    {
        if (args.pos.size() != 3)
        {
            throw ssp21::Exception("Required positional arguments: <input icf file> <input icf file> <output icf file>");
        }

        Actions::append_certificate_chains(args.pos[0], args.pos[1], args.pos[2]);
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


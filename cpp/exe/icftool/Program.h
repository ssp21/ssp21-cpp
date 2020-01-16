
#ifndef ICFTOOL_PROGRAM_H
#define ICFTOOL_PROGRAM_H

#include <argagg/argagg.hpp>

namespace flags {
static const char* const help = "help";
static const char* const show = "show";
static const char* const x25519 = "x25519";
static const char* const ed25519 = "ed25519";
static const char* const shared = "shared";
static const char* const cert = "cert";
static const char* const append = "append";
static const char* const days = "days";
static const char* const level = "level";
}

class Program {

public:
    Program();

    void run(int argc, char* argv[]);

private:
    void print_help();

    argagg::parser parser;
};

#endif
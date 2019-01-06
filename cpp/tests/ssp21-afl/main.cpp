
#include <ssp21/crypto/gen/RequestHandshakeBegin.h>
#include <ssp21/crypto/gen/ReplyHandshakeBegin.h>
#include <ssp21/crypto/gen/SessionData.h>
#include <ssp21/crypto/gen/CertificateEnvelope.h>
#include <ssp21/crypto/gen/CertificateBody.h>

#include <ssp21/util/ConsolePrettyPrinter.h>
#include <ssp21/crypto/LogMessagePrinter.h>
#include <ssp21/stack/LogLevels.h>
#include <ssp21/util/Exception.h>

#include <iostream>
#include <fstream>

using namespace ssp21;

template <class T>
void parse_input_as(const seq32_t& input, IMessagePrinter& printer)
{
    seq32_t copy = input;
    T message;
    const auto err = message.read(copy);
    if(!any(err))
    {
        message.print(printer);
    }
}

template <class T>
void parse_input_as(const seq32_t& input, const char* name, IMessagePrinter& printer)
{
    seq32_t copy = input;
    T message;
    const auto err = message.read(copy);
    if(!any(err))
    {
        message.print(name, printer);
    }
}

template <class T>
void write_default_seed(const std::string& directory, const std::string& filename)
{
    const uint32_t max_bytes = 256;
    uint8_t buffer[max_bytes];

    wseq32_t output(buffer, max_bytes);
    T message;
    const auto result = message.write(output);
    if(any(result.err))
    {
        throw Exception("unable to write default seed: ", FormatErrorSpec::to_string(result.err));
    }

    const auto path = directory + "/" + filename;

    std::ofstream file(path, std::ios::out | std::ios::binary | std::ios::trunc);
    if(!file.is_open()) {
        throw Exception("unable to open seed file for writing: ", path);
    }


    file.write(reinterpret_cast<const char*>(buffer), result.written.length());
}

template <class T>
void write_default_seed_field(const std::string& directory, const std::string& filename)
{
    const uint32_t max_bytes = 256;
    uint8_t buffer[max_bytes];

    wseq32_t output(buffer, max_bytes);
    T message;
    const auto err = message.write(output);
    if(any(err))
    {
        throw Exception("unable to write default seed: ", FormatErrorSpec::to_string(err));
    }

    const auto path = directory + "/" + filename;

    std::ofstream file(path, std::ios::out | std::ios::binary | std::ios::trunc);
    if(!file.is_open()) {
        throw Exception("unable to open seed file for writing: ", path);
    }

    //figure out how much was written to the output buffer
    const auto length = max_bytes - output.length();
    file.write(reinterpret_cast<const char*>(buffer), length);
}

int perform_fuzzing()
{
    // read stdin
    const size_t max_bytes = 1024;
    uint8_t buffer[max_bytes];
    const auto count = std::fread(buffer, sizeof(uint8_t), max_bytes, stdin);
    const ssp21::seq32_t slice(buffer, static_cast<uint32_t>(count));

    // set up pretty printing
    const auto console_logger = std::make_shared<ConsolePrettyPrinter>();
    openpal::Logger logger(
            std::make_shared<ConsolePrettyPrinter>(),
            ssp21::Module::id,
            "fuzz",
            openpal::LogLevels::everything()
    );
    LogMessagePrinter printer(logger, levels::event);

    // try the input for each of the message / cert types
    parse_input_as<RequestHandshakeBegin>(slice, printer);
    parse_input_as<ReplyHandshakeBegin>(slice, printer);
    parse_input_as<SessionData>(slice, printer);
    parse_input_as<CertificateEnvelope>(slice, "envelope", printer);
    parse_input_as<CertificateBody>(slice, "body", printer);

    return 0;
}

int write_seeds(const std::string& directory)
{
    write_default_seed<RequestHandshakeBegin>(directory, "seed1.bin");
    write_default_seed<ReplyHandshakeBegin>(directory, "seed2.bin");
    write_default_seed<SessionData>(directory, "seed3.bin");
    write_default_seed_field<CertificateEnvelope>(directory, "seed4.bin");
    write_default_seed_field<CertificateBody>(directory, "seed5.bin");

    return 0;
}

int main(int argc, char*  argv[])
{
    return (argc > 1) ? write_seeds(argv[1]) : perform_fuzzing();
}

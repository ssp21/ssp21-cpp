
#include <ssp21/crypto/gen/RequestHandshakeBegin.h>
#include <ssp21/crypto/gen/ReplyHandshakeBegin.h>
#include <ssp21/crypto/gen/SessionData.h>
#include <ssp21/crypto/gen/CertificateEnvelope.h>
#include <ssp21/crypto/gen/CertificateBody.h>

#include <ssp21/util/ConsolePrettyPrinter.h>
#include <ssp21/crypto/LogMessagePrinter.h>
#include <ssp21/stack/LogLevels.h>


using namespace ssp21;

template <class T>
void parse_input_as(const seq32_t& input, IMessagePrinter& printer)
{
    seq32_t copy = input;
    T message;
    const auto err = message.read(copy);
    if(!any(err)) {
        message.print(printer);
    }
}

template <class T>
void parse_input_as(const seq32_t& input, const char* name, IMessagePrinter& printer)
{
    seq32_t copy = input;
    T message;
    const auto err = message.read(copy);
    if(!any(err)) {
        message.print(name, printer);
    }
}

int main(int argc, char*  argv[])
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
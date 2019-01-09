
#include <iostream>

#include <asio.hpp>
#include <exe4cpp/asio/BasicExecutor.h>

#include <ssp21/util/ConsolePrettyPrinter.h>
#include <ssp21/stack/LogLevels.h>
#include <sodium/SodiumBackend.h>
#include <openpal/logging/LogMacros.h>

#include "ConfigReader.h"
#include "Proxy.h"

using namespace std;
using namespace ssp21;
using namespace openpal;

void run(const std::string& config_file_path);

int main(int argc, char*  argv[])
{
    if (!ssp21::SodiumBackend::initialize())
    {
        cerr << "Unable to initialze the crypto backend" << endl;
        return -1;
    }

    if (argc != 2)
    {
        cerr << "Usage: ssp21-proxy <path to ini file>" << endl;
        return -1;
    }

    try
    {
        run(argv[1]);
    }
    catch (const exception& ex)
    {
        cerr << ex.what() << endl;
        return -1;
    }

    return 0;
}

std::shared_ptr<openpal::ILogHandler> get_log_backend()
{
    ConsolePrettyPrinter::Settings settings;
    settings.max_id_size = 20;
    return make_shared<ConsolePrettyPrinter>(settings);
}

void run(const std::string& config_file_path)
{
    // setup the logging backend
    Logger logger(get_log_backend(), Module::id, "ssp21-proxy", LogLevels(~0));

    auto configurations = ConfigReader::read(logger, config_file_path);

    if (configurations.empty())
    {
        throw std::logic_error("no proxy configurations were specified");
    }

    auto service = std::make_shared<asio::io_service>();
    const auto executor = make_shared<exe4cpp::BasicExecutor>(service);

    std::vector<std::unique_ptr<Proxy>> proxies;

    // initialize all the proxies. might throw on bad configuration.
    for (auto& config : configurations)
    {
        FORMAT_LOG_BLOCK(logger, ssp21::levels::event, "initializing proxy: %s", config->id.c_str());

        proxies.push_back(
            std::make_unique<Proxy>(
                *config,
                executor,
                logger.detach(config->id, config->log_levels)
            )
        );
    }

    // start all the sessions
    for (auto& p : proxies) p->start();

    SIMPLE_LOG_BLOCK(logger, ssp21::levels::event, "begin io_context::run()");
    executor->get_service()->run();
    SIMPLE_LOG_BLOCK(logger, ssp21::levels::event, "end io_context::run()");
}

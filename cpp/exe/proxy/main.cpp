
#include <iostream>

#include <asio.hpp>

#include <ssp21/util/ConsolePrettyPrinter.h>
#include <ssp21/stack/LogLevels.h>
#include <ssp21/crypto/Crypto.h>

#include <openpal/logging/LogMacros.h>

#include "ConfigReader.h"
#include "Executor.h"
#include "Proxy.h"

using namespace std;
using namespace ssp21;
using namespace openpal;

void run(const vector<unique_ptr<ProxyConfig>>& config);

int main(int argc, char*  argv[])
{
    if (!ssp21::Crypto::initialize())
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
        run(ConfigReader::read(argv[1]));
    }
    catch (const exception& ex)
    {
        cerr << ex.what() << endl;
        return -1;
    }

    return 0;
}

void run(const vector<unique_ptr<ProxyConfig>>& config)
{
    if (config.empty())
    {
        throw std::logic_error("no proxy configurations were specified");
    }

    ConsolePrettyPrinter::Settings settings;
    settings.max_id_size = 20;

    Logger logger(make_shared<ConsolePrettyPrinter>(settings), Module::id, "ssp21-proxy", LogLevels(~0));

    const auto executor = make_shared<Executor>();

    std::vector<std::unique_ptr<Proxy>> proxies;

    // initialize all the proxies. might throw on bad configuration.
    for (auto& c : config)
    {
        FORMAT_LOG_BLOCK(logger, ssp21::levels::event, "initializing proxy: %s", c->id.c_str());

        proxies.push_back(
            std::make_unique<Proxy>(
                *c,
                executor,
                logger.detach(c->id, c->log_levels)
            )
        );
    }

    // start all the sessions
    for (auto& p : proxies) p->start();

    SIMPLE_LOG_BLOCK(logger, ssp21::levels::event, "begin io_context::run()");
    executor->run();
    SIMPLE_LOG_BLOCK(logger, ssp21::levels::event, "end io_context::run()");
}

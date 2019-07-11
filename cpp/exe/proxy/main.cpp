
#include <iostream>

#include <asio.hpp>
#include <exe4cpp/asio/BasicExecutor.h>

#include <log4cpp/ConsolePrettyPrinter.h>
#include <ssp21/stack/LogLevels.h>
#include <sodium/CryptoBackend.h>
#include <log4cpp/LogMacros.h>

#include "ConfigReader.h"
#include "tcp/TcpProxy.h"
#include "udp/UdpProxy.h"

using namespace std;
using namespace ssp21;

void run(const std::string& config_file_path);

int main(int argc, char*  argv[])
{
    if (!ssp21::sodium::CryptoBackend::initialize())
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

std::shared_ptr<log4cpp::ILogHandler> get_log_backend()
{
    log4cpp::ConsolePrettyPrinter::Settings settings;
    settings.max_id_size = 20;
    return make_shared<log4cpp::ConsolePrettyPrinter>(settings);
}

std::unique_ptr<Proxy> create_proxy(log4cpp::Logger& logger, std::shared_ptr<exe4cpp::BasicExecutor> executor, const ProxyConfig& config)
{
    if(config.proto_type == ProxyConfig::ProtoType::tcp)
    {
        FORMAT_LOG_BLOCK(logger, ssp21::levels::event, "initializing TCP proxy: %s", config.id.c_str());

        return std::make_unique<TcpProxy>(
            static_cast<const TcpProxyConfig&>(config),
            executor,
            logger.detach(config.id, config.log_levels)
        );
    }
    else
    {
        FORMAT_LOG_BLOCK(logger, ssp21::levels::event, "initializing UDP proxy: %s", config.id.c_str());

        return std::make_unique<UdpProxy>(
            static_cast<const UdpProxyConfig&>(config),
            executor,
            logger.detach(config.id, config.log_levels)
        );
    }
    
}

void run(const std::string& config_file_path)
{
    // setup the logging backend
    log4cpp::Logger logger(get_log_backend(), Module::id, "ssp21-proxy", log4cpp::LogLevels::everything());

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
        proxies.push_back(create_proxy(logger, executor, *config));
    }

    // start all the sessions
    for (auto& p : proxies) p->start();

    SIMPLE_LOG_BLOCK(logger, ssp21::levels::event, "begin io_context::run()");
    executor->get_service()->run();
    SIMPLE_LOG_BLOCK(logger, ssp21::levels::event, "end io_context::run()");
}

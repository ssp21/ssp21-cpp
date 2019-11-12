
#include <iostream>

#include <asio.hpp>
#include <exe4cpp/asio/BasicExecutor.h>

#include <log4cpp/ConsolePrettyPrinter.h>
#include <ssp21/stack/LogLevels.h>
#include <ssp21/stack/Version.h>
#include <sodium/CryptoBackend.h>
#include <log4cpp/LogMacros.h>

#include "ProxyConfig.h"
#include "tcp/TcpProxySession.h"
#include "udp/UdpProxySession.h"

using namespace std;
using namespace ssp21;

void run(const std::string& config_file_path);

int main(int argc, char*  argv[])
{
    if (!ssp21::sodium::CryptoBackend::initialize())
    {
        cerr << "Unable to initialize the crypto backend" << endl;
        return -1;
    }

    if (argc != 2)
    {
        cerr << "Usage:" << endl << endl;
        cerr << "ssp21-proxy -v      # prints version info" << endl;
        cerr << "ssp21-proxy <path>  # runs the proxy with specified configuration file" << endl;
        return -1;
    }

    if (strcmp(argv[1], "-v") == 0)
    {
        cout << "git commit date: " << version::git_commit_date() << std::endl;
        cout << "git commit hash: " << version::git_commit_hash() << std::endl;
        return 0;
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

std::vector<std::unique_ptr<IProxySession>> configure_sessions(
    const std::string& config_file_path,
    const log4cpp::Logger& logger, 
    const std::shared_ptr<exe4cpp::BasicExecutor>& executor)
{
    const auto factories = config::read(config_file_path, executor, logger);

    if (factories.empty())
    {
        throw std::logic_error("no proxy sessions were specified");
    }

    std::vector<std::unique_ptr<IProxySession>> sessions;

    // initialize all the proxies. might throw on bad configuration.
    for (auto& factory : factories)
    {
        sessions.push_back(factory(logger, executor));
    }

    return sessions;
}

void run(const std::string& config_file_path)
{
    // setup the logging backend
    log4cpp::Logger logger(get_log_backend(), Module::id, "ssp21-proxy", log4cpp::LogLevels::everything());
   
    const auto executor = exe4cpp::BasicExecutor::create(std::make_shared<asio::io_service>());
    
    const auto sessions = configure_sessions(config_file_path, logger, executor);

    // start all the sessions
    for (auto& s : sessions) s->start();

    // run the event loop
    SIMPLE_LOG_BLOCK(logger, ssp21::levels::event, "begin io_context::run()");
    executor->get_service()->run();
    SIMPLE_LOG_BLOCK(logger, ssp21::levels::event, "end io_context::run()");
}

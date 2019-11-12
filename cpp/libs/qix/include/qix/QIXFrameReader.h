#ifndef SSP21PROXY_QIXFRAMEREADER_H
#define SSP21PROXY_QIXFRAMEREADER_H

#include <string>
#include <thread>

#include "log4cpp/Logger.h"
#include "IQIXFrameHandler.h"
#include "SerialSettings.h"

#include <exe4cpp/asio/BasicExecutor.h>

class QIXFrameReader
{
public:

    QIXFrameReader(const std::shared_ptr<IQIXFrameHandler>& handler, const log4cpp::Logger& logger, const SerialSettings& settings);
    ~QIXFrameReader();

private:

    const std::shared_ptr<exe4cpp::BasicExecutor> executor;
    std::unique_ptr<std::thread> thread;

    void run_and_catch();
};

#endif

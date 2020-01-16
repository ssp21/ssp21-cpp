#ifndef SSP21PROXY_ASYNCQIXFRAMEREADER_H
#define SSP21PROXY_ASYNCQIXFRAMEREADER_H

#include "IQIXFrameHandler.h"
#include "SerialSettings.h"

#include <exe4cpp/asio/BasicExecutor.h>
#include <log4cpp/Logger.h>

#include <memory>

namespace qix {
namespace async {

    /**
		* Launch a frame handler onto executor
		*/
    void start_frame_reader(
        const std::shared_ptr<IQIXFrameHandler>& handler,
        const log4cpp::Logger& logger,
        const std::shared_ptr<exe4cpp::BasicExecutor>& executor,
        const SerialSettings& settings);

}
}

#endif

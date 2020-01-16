
#ifndef SSP21_SESSIONS_H
#define SSP21_SESSIONS_H

#include "crypto/Session.h"
#include "ser4cpp/util/Uncopyable.h"

namespace ssp21 {
/**
    	Structure that contains both a pending and an active session
    */
class Sessions final : private ser4cpp::Uncopyable {

public:
    Sessions() = delete;

    Sessions(
        const std::shared_ptr<IFrameWriter>& frame_writer,
        const std::shared_ptr<SessionStatistics>& stats,
        const SessionConfig& config);

    void reset_both();
    void activate_pending();

    std::unique_ptr<Session> active;
    std::unique_ptr<Session> pending;
};

}

#endif

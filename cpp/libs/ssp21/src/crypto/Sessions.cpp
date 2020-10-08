
#include "Sessions.h"

namespace ssp21 {
Sessions::Sessions(const std::shared_ptr<IFrameWriter>& frame_writer, const std::shared_ptr<SessionStatistics>& stats, const SessionConfig& config)
    : active(std::make_unique<Session>(frame_writer, stats, config))
    , pending(std::make_unique<Session>(frame_writer, stats, config))
{
}

void Sessions::reset_both()
{
    if (this->active) {
        this->active->reset();
    }
    if(this->pending) {
        this->pending->reset();
    }
}

void Sessions::activate_pending()
{
    this->active->reset();
    this->active.swap(pending);
}

}

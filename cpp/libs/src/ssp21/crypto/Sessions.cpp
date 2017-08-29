
#include "Sessions.h"

namespace ssp21
{
    Sessions::Sessions(const std::shared_ptr<IFrameWriter>& frame_writer, const SessionConfig& config) :
        active(std::make_unique<Session>(frame_writer, config)),
        pending(std::make_unique<Session>(frame_writer, config))
    {

    }

    void Sessions::reset_both()
    {
        this->active->reset();
        this->pending->reset();
    }

    void Sessions::activate_pending()
    {
        this->active->reset();
        this->active.swap(pending);
    }

}



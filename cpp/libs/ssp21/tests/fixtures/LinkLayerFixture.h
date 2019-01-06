

#ifndef SSP21_LINKLAYERFIXTURE_H
#define SSP21_LINKLAYERFIXTURE_H

#include "mocks/MockLogHandler.h"
#include "mocks/MockLowerLayer.h"
#include "mocks/MockUpperLayer.h"

#include "link/LinkLayer.h"

namespace ssp21
{
    class LinkLayerFixture
    {

    public:

        LinkLayerFixture() : log("responder"), link(1, 10)
        {
            upper.bind_lower(link);
            lower.bind_upper(link);
            link.bind(lower, upper);
        }

        ssp21::MockLogHandler log;
        MockLowerLayer lower;
        MockUpperLayer upper;
        LinkLayer link;

    };
}

#endif

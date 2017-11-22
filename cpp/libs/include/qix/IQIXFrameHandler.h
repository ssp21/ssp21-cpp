#ifndef SSP21PROXY_IQIXFRAMEHANDLER_H
#define SSP21PROXY_IQIXFRAMEHANDLER_H

#include "QIXFrame.h"

class IQIXFrameHandler
{
public:

	virtual ~IQIXFrameHandler() {}

	virtual void handle(const QIXFrame& frame) = 0;

};

#endif

#ifndef SSP21PROXY_IPROXYSESSION_H
#define SSP21PROXY_IPROXYSESSION_H

class IProxySession
{
public:
    virtual ~IProxySession() = default;
    virtual void start() = 0;
};

#endif

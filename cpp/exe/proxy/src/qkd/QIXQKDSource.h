#ifndef SSP21PROXY_QIXQKDSOURCE_H
#define SSP21PROXY_QIXQKDSOURCE_H

#include "IQKDSource.h"

#include <log4cpp/Logger.h>
#include <qix/QIXFrameReader.h>
#include <qix/QIXKeyStore.h>
#include <exe4cpp/asio/BasicExecutor.h>

#include <yaml-cpp/yaml.h>

#include <map>
#include <mutex>

class QIXQKDSource : public IQKDSource
{    
	struct FrameHandler : public IQIXFrameHandler
	{	    
        FrameHandler(uint16_t key_cycle_length);

		void handle(const QIXFrame& frame) override;

        const uint16_t num_subscribers;
        std::map<uint16_t, std::shared_ptr<QIXKeyStore>> subscribers;
	};

public:

	QIXQKDSource(const YAML::Node& node, const std::shared_ptr<exe4cpp::BasicExecutor>& executor, log4cpp::Logger& logger);
	
	std::shared_ptr<ssp21::IKeyLookup> bind_responder_key_lookup(uint16_t modulo, uint16_t max_cached_keys) override;
	
	std::shared_ptr<ssp21::IKeySource> bind_initiator_key_source(uint16_t modulo, uint16_t max_cached_keys) override;

private:

    std::shared_ptr<QIXKeyStore> bind_key_store(uint16_t modulo, uint16_t max_cached_keys);


	std::shared_ptr<FrameHandler> handler;    	
};

#endif

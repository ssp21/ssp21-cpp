set(proxy_headers
    ./src/AsioLowerLayer.h
    ./src/AsioUpperLayer.h    
    ./src/ConfigReader.h    
    ./src/IAsioLayer.h
    ./src/IPEndpoint.h
    ./src/IProxySession.h	
    ./src/LogConfig.h
    ./src/ProxyConfig.h
    ./src/ProxySessionFactory.h	
    ./src/Session.h
    ./src/StackConfigReader.h
    ./src/StackFactory.h
    ./src/YAMLHelpers.h
    
	./src/qkd/IQKDSource.h
	./src/qkd/QIXQKDSource.h
	./src/qkd/QKDSourceRegistry.h

    ./src/tcp/AsioTcpSocketWrapper.h
    ./src/tcp/TcpConfig.h
    ./src/tcp/TcpProxySession.h	

    ./src/udp/AsioUdpSocketWrapper.h
    ./src/udp/UdpConfig.h
    ./src/udp/UdpProxySession.h
)

set(proxy_srcs
    ./src/main.cpp

    ./src/ConfigReader.cpp    
    ./src/IPEndpoint.cpp
    ./src/LogConfig.cpp
    ./src/ProxyConfig.cpp	
    ./src/Session.cpp
    ./src/StackConfigReader.cpp
    ./src/YAMLHelpers.cpp

    ./src/tcp/TcpConfig.cpp
    ./src/tcp/TcpProxySession.cpp

    ./src/udp/UdpConfig.cpp
    ./src/udp/UdpProxySession.cpp	

	./src/qkd/QIXQKDSource.cpp
	./src/qkd/QKDSourceRegistry.cpp
)

add_executable(proxy ${proxy_headers} ${proxy_srcs})
target_include_directories(proxy PRIVATE ./src)
target_link_libraries(proxy PRIVATE ssp21 sodium_backend qix asio yaml-cpp)
clang_format(proxy)

install(TARGETS proxy EXPORT Ssp21Targets
    RUNTIME DESTINATION bin
)

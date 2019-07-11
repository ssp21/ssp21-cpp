set(proxy_headers
    ./AsioLowerLayer.h
    ./AsioUpperLayer.h
    ./ConfigProperties.h
    ./ConfigReader.h
    ./ConfigSection.h
    ./IAsioLayer.h
    ./ProxyConfig.h
    ./Session.h
    ./StackFactory.h

    ./tcp/AsioTcpSocketWrapper.h
    ./tcp/TcpProxy.h

    ./udp/AsioUdpSocketWrapper.h
    ./udp/UdpProxy.h
)

set(proxy_srcs
    ./main.cpp

    ./ConfigReader.cpp
    ./ConfigSection.cpp
    ./Session.cpp

    ./tcp/TcpProxy.cpp

    ./udp/UdpProxy.cpp
)

add_executable(proxy ${proxy_headers} ${proxy_srcs})
target_include_directories(proxy PRIVATE .)
target_link_libraries(proxy PRIVATE ssp21 sodium_backend qix asio inih)

install(TARGETS proxy EXPORT Ssp21Targets
    RUNTIME DESTINATION bin
)

set(proxy_headers
    ./ASIOLayerBase.h
    ./ASIOLowerLayer.h
    ./ASIOUpperLayer.h
    ./ConfigProperties.h
    ./ConfigReader.h
    ./ConfigSection.h
    ./Proxy.h
    ./ProxyConfig.h
    ./Session.h
    ./StackFactory.h
)

set(proxy_srcs
    ./main.cpp

    ./ConfigReader.cpp
    ./ConfigSection.cpp
    ./Proxy.cpp
    ./Session.cpp
)

add_executable(proxy ${proxy_headers} ${proxy_srcs})
target_include_directories(proxy PRIVATE .)
target_link_libraries(proxy PRIVATE ssp21 sodium_backend qix asio inih)

install(TARGETS proxy EXPORT Ssp21Targets
    RUNTIME DESTINATION bin
)
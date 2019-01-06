set(proxy_headers
    ./ASIOLayerBase.h
    ./ASIOLowerLayer.h
    ./ASIOUpperLayer.h
    ./ConfigProperties.h
    ./ConfigReader.h
    ./ConfigSection.h
    ./Executor.h
    ./Proxy.h
    ./ProxyConfig.h
    ./Session.h
    ./StackFactory.h
)

set(proxy_srcs
    ./main.cpp

    ./ConfigReader.cpp
    ./ConfigSection.cpp
    ./Executor.cpp
    ./Proxy.cpp
    ./Session.cpp
)

add_library(proxy ${proxy_headers} ${proxy_srcs})
target_include_directories(proxy PRIVATE .)
target_link_libraries(proxy PRIVATE ssp21 sodium_backend qix asio inih)

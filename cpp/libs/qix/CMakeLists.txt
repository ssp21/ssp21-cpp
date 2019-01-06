set(qix_public_headers
    ./include/qix/IQIXFrameHandler.h
    ./include/qix/QIXFrame.h
    ./include/qix/QIXFrameParser.h
    ./include/qix/QIXFrameReader.h
    ./include/qix/QIXKeyCache.h
)

set(qix_srcs
    ./src/QIXFrameParser.cpp
    ./src/QIXFrameReader.cpp
    ./src/QIXKeyCache.cpp
)

add_library(qix ${qix_public_headers} ${qix_srcs})
target_include_directories(qix PUBLIC ./include PRIVATE ./src)
target_link_libraries(qix PUBLIC ssp21 asio)

add_subdirectory(./tests)

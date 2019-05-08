set(qix_public_headers
    ./include/qix/IQIXFrameHandler.h
    ./include/qix/QIXFrame.h
    ./include/qix/QIXFrameParser.h
    ./include/qix/QIXFrameReader.h
    ./include/qix/QIXFrameWriter.h
    ./include/qix/QIXKeyCache.h
)

set(qix_srcs
    ./src/QIXFrameParser.cpp
    ./src/QIXFrameReader.cpp
    ./src/QIXFrameWriter.cpp
    ./src/QIXKeyCache.cpp
)

add_library(qix ${qix_public_headers} ${qix_srcs})
target_include_directories(qix
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE ./src
)
target_link_libraries(qix PUBLIC ssp21 asio)

#install(TARGETS qix EXPORT Ssp21Targets
#    LIBRARY DESTINATION lib
#    ARCHIVE DESTINATION lib
#)
#install(FILES ${qix_public_headers} DESTINATION include/qix)

add_subdirectory(./tests)

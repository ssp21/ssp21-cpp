set(sodium_backend_public_headers
    ./include/sodium/CryptoBackend.h
)

set(sodium_backend_private_headers
    ./src/HKDF.h
)

set(sodium_backend_srcs
    ./src/CryptoBackend.cpp
)

add_library(sodium_backend ${sodium_backend_public_headers} ${sodium_backend_private_headers} ${sodium_backend_srcs})
target_include_directories(sodium_backend
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE ./src
)
target_link_libraries(sodium_backend PUBLIC ssp21 PRIVATE sodium)
clang_format(sodium_backend)

install(TARGETS sodium_backend EXPORT Ssp21Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(DIRECTORY ./include/sodium DESTINATION include)

add_subdirectory(./tests)

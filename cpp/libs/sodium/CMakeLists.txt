set(sodium_backend_public_headers
    ./include/sodium/SodiumBackend.h
)

set(sodium_backend_private_headers
    ./src/HKDF.h
)

set(sodium_backend_srcs
    ./src/SodiumBackend.cpp
)

add_library(sodium_backend ${sodium_backend_public_headers} ${sodium_backend_private_headers} ${sodium_backend_srcs})
target_include_directories(sodium_backend PUBLIC ./include PRIVATE ./src)
target_link_libraries(sodium_backend PUBLIC ssp21 PRIVATE sodium)

add_subdirectory(./tests)

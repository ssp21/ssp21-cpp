set(icftool_headers
    ./Actions.h
    ./ConsolePrinter.h
    ./Program.h
    ./UserInput.h
)

set(icftool_srcs
    ./main.cpp

    ./Actions.cpp
    ./Program.cpp
    ./UserInput.cpp
)

add_library(icftool ${icftool_headers} ${icftool_srcs})
target_include_directories(icftool PRIVATE .)
target_link_libraries(icftool PRIVATE ssp21 sodium_backend argagg)

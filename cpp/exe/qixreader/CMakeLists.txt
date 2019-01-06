set(qixreader_srcs
    ./main.cpp
)

add_library(qixreader ${qixreader_srcs})
target_link_libraries(qixreader PRIVATE qix)

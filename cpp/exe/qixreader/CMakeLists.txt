set(qixreader_srcs
    ./main.cpp
)

add_executable(qixreader ${qixreader_srcs})
target_link_libraries(qixreader PRIVATE qix)

install(TARGETS qixreader EXPORT Ssp21Targets
    RUNTIME DESTINATION bin
)

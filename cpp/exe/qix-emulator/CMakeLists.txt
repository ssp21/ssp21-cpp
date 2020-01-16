
add_executable(qix-emulator main.cpp)
target_link_libraries(qix-emulator PRIVATE qix sodium_backend argagg)
clang_format(qix-emulator)

install(TARGETS qix-emulator EXPORT Ssp21Targets
    RUNTIME DESTINATION bin
)

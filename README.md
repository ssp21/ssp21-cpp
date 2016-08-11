## Dependencies

SSP21 depends on [libsodium](https://download.libsodium.org/doc/).

On Windows, download and install the prebuilt binaries. You'll need to tell cmake where to find the headers and correct library.
For instance, if you're building a 32-bit debug build, you can invoke cmake with:

```
> cmake .. \
-DSSP21_LIBSODIUM_INCLUDE_DIR=C:\libs\libsodium-1.0.10-msvc\include \
-DSSP21_LIBSODIUM_LIB_DIR=C:\libs\libsodium-1.0.10-msvc\Win32\Debug\v120\static
```

On Linux, your best bet is to build libsodium from source and install it.

## Coding style

We're playing around with the [CppCoreGuidelines](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines) right now, but this is
subject to change.

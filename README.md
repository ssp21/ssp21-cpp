## Dependencies

SSP21 depends on [libsodium](https://download.libsodium.org/doc/). You can substitute another cryptographic backend if you really must.

On Windows, download and install the prebuilt binaries. You'll need to tell cmake where to find the distribution:

```
> cmake .. -DSSP21_SODIUM_DIR=C:\libs\libsodium-1.0.10-msvc
```

On Linux, your best bet is to build libsodium from source and install it.

## Coding style

We're playing around with the [CppCoreGuidelines](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines) right now, but this is
subject to change.

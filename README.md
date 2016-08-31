## About

A reference implementation of Secure SCADA Protocol for the 21st century (SSP21) in C++.  

## Dependencies

### openpal

[openpal](https://github.com/automatak/openpal) is a BSD licensed API for abstracting protocol stack development.

The dependency is defined as a git submodule and built from source. You can check ssp21-cpp out recursively to checkout 
openpal automatically.

```
git clone --recursive https://github.com/aegis4ics/ssp21-cpp.git
```

### libsodium 

SSP21 depends on [libsodium](https://download.libsodium.org/doc/). You can substitute another cryptographic backend if you really must.

On Windows, download and install the prebuilt binaries. You'll need to tell cmake where to find the distribution:

```
> cmake .. -DSSP21_SODIUM_DIR=C:\libs\libsodium-1.0.10-msvc
```

On Linux, your best bet is to build libsodium from source and install it.

## Coding style and guidelines

We're using the [CppCoreGuidelines](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines).

## Formatting

There's a custom build target called "FORMAT" that invokes [AStyle](http://astyle.sourceforge.net/) and expects
to find it on your PATH.

Please install version **2.05** as there are subtle differences between versions.

Please format code using this target before making any commits.

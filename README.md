## About

A reference implementation of Secure SCADA Protocol for the 21st century (SSP21) in C++.  

## Dependencies

### ser4cpp, exe4cpp, log4cpp

[ser4cpp](https://github.com/automatak/ser4cpp) is a BSD licensed API for safe low-level serialization routines.

[exe4cpp](https://github.com/automatak/exe4cpp) is a BSD licensed event-loop executor abstractions based on ASIO.

[log4cpp](https://github.com/automatak/log4cpp) is a BSD licensed API for logging with extensible log levels using `ser4cpp` sequence types.

All these dependencies are header-only and defined as a git submodule and built from source. You can check ssp21-cpp out recursively to checkout 
the dependencies automatically.

```
git clone --recursive https://github.com/aegis4ics/ssp21-cpp.git
```

### libsodium 

SSP21 depends on [libsodium](https://download.libsodium.org/doc/). You can substitute another cryptographic backend if you really must.

On Windows, download and install the prebuilt binaries. You'll need to tell cmake where to find the distribution:

```
> cmake .. -Dsodium_DIR=C:\libs\libsodium-1.0.10-msvc
```

On Linux, your best bet is to build libsodium from source and install it.

## Coding style and guidelines

We're using the [CppCoreGuidelines](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines).

## Formatting

There's a custom build target called "FORMAT" that invokes [AStyle](http://astyle.sourceforge.net/) and expects
to find it on your PATH.

Please install version **2.05** as there are subtle differences between versions.

Please format code using this target before making any commits.

## Documentation
To build the basic Doxygen documentation, simply run `doxygen` in the root
directory of the repository. The documentation will be generated in the
`build/doc` folder.

To build the documentation with the [custom
theme](http://mcss.mosra.cz/doxygen/), the following dependencies are needed:
* Doxygen 1.8.14 or higher
* Python 3.6
* Jinja2 (installable by `pip`)
* Pygments (installable by `pip`)
* m.css submodule (available in this repository)

A `pipenv` configuration is available in the `/doc` directory.

A Docker image with all the required dependency is available here:
https://hub.docker.com/r/emgre/circleci-doxygen/

To build the documentation with the theme, simply run
`deps/m.css/doxygen/dox2html5.py Doxyfile-mcss`. The documentation will be
generated in the `build/doc` folder. You can also run the `doc` CMake target to
run the command.

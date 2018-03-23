# MidiUtils - Utils Library for Standard Midi File Processing

*(Still WIP Project)*

## Overview

MidiUtils is a C++ library for parsing and processing Standard Midi Files.

## Build

You need [Premake4](https://premake.github.io/) 4.4 or later version of Premake to build this library.

Change directory to the project directory and use the `premake4 --help` command will show all the `premake4` supported target platform (or using `premake5` for more target platform). For example.

``` bash
# show me the help
premake4 --help
# for GNU Makefile under linux, cygwin or msys (and maybe Mac?)
premake4 gmake
# for Visual Studio 2012 under Windows
premake4 vs2012 
```

Then just build the library with whatever you prefer.

``` bash
# build with GNU Makefile
make
```

After build, you will see the output binary located at `build/bin/`.

## Document

Since this is a still WIP project, many things may change. Please wait for more detail or directly dig into the source files.

## License

BSD-2-Clause

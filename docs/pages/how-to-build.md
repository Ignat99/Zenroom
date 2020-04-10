# Build instructions

This section is optional for those who want to build this software from source. The following build instructions contain generic information meant for an expert audience.

!> After cloning this source code from git, one should do:
```bash
git submodule update --init --recursive
```

The Zenroom compiles the same sourcecode to run on Linux in the form of 2 different POSIX compatible ELF binary formats using GCC (linking shared libraries) or musl-libc (fully static) targeting both X86 and ARM architectures.
It also compiles to a Windows 64-bit native and fully static executable. At last, it compiles to Javascript/Webassembly using the LLVM based emscripten SDK. To recapitulate some Makefile targets:

## Shared builds
The simpliest, builds a shared executable linked to a system-wide libc, libm and libpthread (mostly for debugging)
Then first build the shared executable for your platform:

<!-- tabs:start -->

#### ** Linux **

```bash
make linux
```

#### ** macOS **

```bash
make osx
```

#### ** Windows **

```bash
make win # builds a Windows 64bit executable with no DLL dependancy, containing the LUA interpreter and all crypto functions (for client side operations on windows desktops)
```

#### ** BSD **
```bash
make bsd
```

<!-- tabs:end -->


To run tests:

<!-- tabs:start -->

#### ** Functional **

```bash
make check-osx
make check-linux
```

#### ** Integration **

```bash
make check-js
make check-py
```

#### **Crypto**
```bash
make check-crypto
make check-crypto-lw
```

<!-- tabs:end -->

## Static builds
Builds a fully static executable linked to musl-libc (to be operated on embedded platforms).

To build the static environment:

```
make bootstrap
make static
make check-static
```


## Javascript builds

For the Javascript and WebAssembly modules the Zenroom provides various targets provided by emscripten which must be installed and loaded in the environment according to the emsdk's instructions and linked inside the `build` directory of zenroom sources:

!> (need EMSDK env) builds different flavors of Javascript modules to be operated from a browser or NodeJS

```bash
make javascript-wasm # For the webassembly build node/web
make javascript-rn   # For react native
```

There is another target to create the [`playground`](https://dev.zenroom.org/demo/)
locally a simple web page with a REPL and some boilerplate code to show how to
use the WebAssembly binary.

!> for the `javascript-demo` target the generated files should be served by a http server

```bash
make javascript-demo
cd docs
make preview
```

then point your browser to http://localhost:3000/demo

## Build instructions for Mobile libraries

### iOS

You need to have install `Xcode` with the `commandline-tools`

There are 3 different targets `ios-sim` `ios-armv7` `ios-arm64` these targets creates an static library with the correct architecture (x86_64, ARMV7, ARM64).

Finally once done all the libraries there is a final target `ios-fat` that put them together creating a fat-binary that you can include into your app. 

Or you can just use the `build-ios.sh` that does all the steps for you!

For using the library just copy `zenroom.a` somewhere in your project and include the zenroom.h file.

### Android

You need to have installed `android-sdk` (if you have Android Studio installed is already there) and set the `ANDROID_HOME` variable.

Also you need to install NDK inside the android-sdk using the Android Studio -> Tools -> Android -> SDK Manager

Finally use the `builld-android.sh` script (be sure that the ANDROID_HOME environment var is set) and you will have at the end `libzenroom-arm.so` and libzenroom-x86.so

To use it in your project just drop `src/Zenroom.java` inside your codebase and the put the `*.so` as following:

```
src/
    main/
         java/
         jniLibs/
                 x86/ 
                      libzenroom.so
                 armeabi/ 
                      libzenroom.so
```
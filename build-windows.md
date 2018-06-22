sssssssssssss# Building XMax Platform on Windows 10

# Prerequisites 

* **Visual Studio 15 2017** Win64 with Visual C++ 2017 15 compiler and tools
* **CMake 3.11.0** or newer
* **Boost 1.67.0**

# Quick Guide

Open command prompt enter the following command to generate Visual Studio Solution

    cmake -G "Visual Studio 15 2017 Win64" -DBOOST_ROOT="[boost dir]" "[source dir]"    
e.g.

    cmake -G "Visual Studio 15 2017 Win64" -DBOOST_ROOT="D:\boost_1_67_0" "../"

Open "Xmax-mainchain.sln" with Visual Studio 2017 and build.

# Detailed Guide

1. [Download & install CMake](#cmake)
1. [Download & build Boost](#boost)
1. [Generate Visual Studio Solution with CMake](#gensln)
1. [Building XMax Platform with Visual Studio 15 2017](#build)
1. [Build MongoDB](#buildmongodb)

# Download & install CMake
<a name="cmake"></a>

Get latest CMake Windows win64-x64 at https://cmake.org/download/ .

Run the installer or unzip to install.

If you choose to download the installer(recommanded). Be sure to check "Add CMake to the system PATH" option while installing so you can use CMake via command prompt. You can also add it to the system PATH manually later, same as while you choose to install via zip.

Note that the directory needs to add to PATH is "bin" which is under the root directory. e.g.

    C:\Program Files\CMake\bin

# Download & build Boost
<a name="boost"></a>

## Install boost from binarie.

Get boost binaries at https://dl.bintray.com/boostorg/release/1.67.0/binaries/ .

Run the installer to install.

Add "BOOST_ROOT" to the system PATH. e.g.

    D:\boost_1_67_0

## Build boost from source

Get boost source at https://dl.bintray.com/boostorg/release/1.67.0/source/ .

Unzip.

Add "BOOST_ROOT" to the system PATH.

Open the command line or Windows PowerShell and cd to boost root path, call build command:
```bash
.\bootstrap.bat --prefix="c:\boost_1_67_0"
.\b2 --build-type=complete toolset=msvc-14.1 address-model=64 --build-dir=.x64 --stagedir=stage_x64 --prefix="c:\boost_1_67_0"  install
```
If you used other source directory, you need to replace the path value in above steps.

# Generate Visual Studio Solution with CMake
<a name="gensln"></a>

Open command prompt from the directory you wish to store the generated solution files. Usually, you would want to store the solution files outside the repostory (i.e. the source directory).

Enter the following command:

    cmake -G "Visual Studio 15 2017 Win64" -DBOOST_ROOT="[boost dir]" "[source dir]"    
e.g.

    E:\XMaxPlatform\sln>cmake -G "Visual Studio 15 2017 Win64" -DBOOST_ROOT="D:\boost_1_67_0" "../xmax-mainchain"

In the example above, "sln" is the directory where you store the generated solution files;  "xmax-mainchain" is the source directory; "D:\boost_1_67_0" is the boost library directory.

Wait for CMake to execute, when it's done you will receive the following message:

    -- Build files have been written to: [solution directory]

If you receive the following message instead:

    cmake is not recognized as an internal or external command, operable program or batch file.

You need add CMake to the system PATH.

# Building XMax Platform with Visual Studio 15 2017
<a name="build"></a>

Simply open generated solution file "Xmax-mainchain.sln" with Visual Studio 15 2017, and select Build->Build Solution(F6) from menu.


<a name="buildmongodb"></a>
# Build MongoDB 

## 1. Build mongo-c-driver
### 1.1 Build libbson in mongo-c-driver
Copy `mongo-c-driver` from `libraries` directory to anywhere you want to compile mongo-c-driver. Then execute:
```bash
cd mongo-c-driver/src/libbson
cmake -G "Visual Studio 15 2017 Win64" "-DCMAKE_INSTALL_PREFIX=C:\mongo-c-driver" "-DCMAKE_BUILD_TYPE=Release"
```
`C:\mongo-c-driver` is default build location and you can change to whatever you want.
Open `Developer Command Prompt for 2017` and goto `mongo-c-driver/src/libbson` then execute:
```bash
msbuild.exe /p:Configuration=Release ALL_BUILD.vcxproj
msbuild.exe /p:Configuration=Release INSTALL.vcxproj
```

### 1.2 Build mongo-c-driver
```bash
cd mongo-c-driver
cmake -G "Visual Studio 15 2017 Win64" "-DENABLE_SSL=WINDOWS" "-DENABLE_SASL=SSPI" "-DCMAKE_INSTALL_PREFIX=C:\mongo-c-driver" "-DCMAKE_PREFIX_PATH=C:\mongo-c-driver" "-DCMAKE_BUILD_TYPE=Release"
```
Make sure the build directory `C:\mongo-c-driver` is same as previous libbson build directory.

Open `Developer Command Prompt for 2017` and goto the `mongo-c-drive` directory then execute:

```bash
msbuild.exe /p:Configuration=Release ALL_BUILD.vcxproj
msbuild.exe /p:Configuration=Release INSTALL.vcxproj
```

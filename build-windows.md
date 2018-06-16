# Building XMax Platform on Windows 10

# Prerequisites 

* **Visual Studio 15 2017** Win64 with Visual C++ 2017 15 compiler and tools
* **CMake 3.11.0** or newer
* **Boost 1.67.0**

# Quick Guide

Open command prompt enters the following command to generate Visual Studio Solution

    cmake -G "Visual Studio 15 2017 Win64" -DBOOST_ROOT="[boost dir]" "[source dir]"    
e.g.

    cmake -G "Visual Studio 15 2017 Win64" -DBOOST_ROOT="D:\boost_1_67_0" "../"

Open "Xmax-mainchain.sln" with Visual Studio 2017 and build.

# Detailed Guide

1. [Download & install CMake](#cmake)
1. [Download & build Boost](#boost)
1. [Generate Visual Studio Solution with CMake](#gensln)
1. [Building XMax Platform with Visual Studio 15 2017](#build)

# Download & install CMake
<a name="cmake"></a>

Get latest CMake Windows win64-x64 at https://cmake.org/download/ .

Run the installer or unzip to intall.

If you choose to download the installer(recommanded). Be sure to check "Add CMake to the system PATH" option while installing so you can use CMake via command prompt. You can also add it to the system PATH manually later, same as while you choose to install via zip.

Note that the directory needs to add to PATH is "bin" which is under the root directory. e.g.

    C:\Program Files\CMake\bin

# Download & build Boost
<a name="boost"></a>

## Download the prerequisites.
Boost version: **1.67.0**

Use the binaries of boost for msvc-14.1(x64), or build from source.

Binaries download link: https://dl.bintray.com/boostorg/release/1.67.0/binaries/

Source download link: https://dl.bintray.com/boostorg/release/1.67.0/source/


## Build boost from source

### 1.
Download the boost source to the directory you want, such as **c:\boost_1_67_0**.

### 2.
Add a system **ENV variable** to this path **BOOST_ROOT=c:\boost_1_67_0**

### 3.
Open the command line or Windows PowerShell and cd to boost root path, call build command:
```bash
.\bootstrap.bat --prefix="c:\boost_1_67_0"
.\b2 --build-type=complete toolset=msvc-14.1 address-model=64 --build-dir=.x64 --stagedir=stage_x64 --prefix="c:\boost_1_67_0"  install
```
If you used other source directory, you need to replace the path value in above steps.

# Generate Visual Studio Solution with CMake
<a name="gensln"></a>

# Building XMax Platform with Visual Studio 15 2017
<a name="build"></a>



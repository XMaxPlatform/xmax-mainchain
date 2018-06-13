# Build Xmax-mainchain on Windows
Build Xmax main blockchain on Windows 10.

# Table of Contents
1. [Prepare](#prepare)
2. [Build Boost](#buildboost)



<a name="parepare"></a>
# Prepare

* IDE: **Visual Studio 2017**
* **CMake 3.11.0** or above
* **Boost 1.67.0**
* Git command tools


<a name="buildboost"></a>
# Build boost libraries

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
.\b2 --build-type=complete toolset=msvc-14.1 address-model=64 --build-dir=.x64 --stagedir=stage_x64 --prefix="c:\Boost\boost_1_67_0"  install
```
If you used other source directory, you need to replace the path value in above steps.




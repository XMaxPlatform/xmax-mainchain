# Building XMax Platform on Windows 10

# Table of contents
1. [Prerequisites](#prerequisites)
2. [Quick Guide](#quickguide)
3. [Download & install CMake](#cmake)
4. [Download & build Boost](#boost)
5. [Generate Visual Studio Solution with CMake](#gensln)
6. [Building XMax Platform with Visual Studio 15 2017](#build)
7. [Build MongoDB](#buildmongodb)
    1. [Build mongo-c-driver](#buildmongodbcdriver)
    2. [Build mongo-cxx-driver](#buildmongodbcxxdriver)
    3. [Use MongoDB in the xmax-mainchain project](#buildmongodbproject)
    4. [Install and run MongoDB service in Windows](#buildmongodbserver)

<a name="prerequisites"></name>
# Prerequisites 

* **Visual Studio 15 2017** Win64 with Visual C++ 2017 15 compiler and tools
* **CMake 3.11.0** or newer
* **Boost 1.67.0**

<a name="quickguide"></name>
# Quick Guide

Open command prompt enter the following command to generate Visual Studio Solution

    cmake -G "Visual Studio 15 2017 Win64" -DBOOST_ROOT="[boost dir]" -DCMAKE_PREFIX_PATH="[mongo_cxx_root];[mongo_c_root]" "[source dir]"    
e.g.

    cmake -G "Visual Studio 15 2017 Win64" -DBOOST_ROOT="D:\boost_1_67_0" -DCMAKE_PREFIX_PATH="c:\mongo-cxx-driver;c:\mongo-c-driver" "../"

Open "Xmax-mainchain.sln" with Visual Studio 2017 and build.


# Download & install CMake
<a name="cmake"></a>

Get latest CMake Windows win64-x64 at https://cmake.org/download/ .

Run the installer or unzip to install.

If you choose to download the installer(recommanded). 
Be sure to check "Add CMake to the system PATH" option while installing so you can use CMake via command prompt. 
You can also add it to the system PATH manually later, same as while you choose to install via zip.

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

Open command prompt from unziped directory enter the following command:

    .\bootstrap.bat --prefix="D:\boost_1_67_0"

Wait a second for it to finish, then enter the following command to build:

    .\b2 --build-type=complete toolset=msvc-14.1 address-model=64 --build-dir=.x64 --stagedir=stage_x64 --prefix="D:\boost_1_67_0" install

# Generate Visual Studio Solution with CMake
<a name="gensln"></a>

Open command prompt from the directory you wish to store the generated solution files. Usually, you would want to store the solution files outside the repostory (i.e. the source directory).

Enter the following command:

    cmake -G "Visual Studio 15 2017 Win64" -DBOOST_ROOT="[boost dir]" -DCMAKE_PREFIX_PATH="[mongo_cxx_root];[mongo_c_root]" "[source dir]"     
e.g.

    E:\XMaxPlatform\sln>cmake -G "Visual Studio 15 2017 Win64" -DBOOST_ROOT="D:\boost_1_67_0" -DCMAKE_PREFIX_PATH="c:\mongo-cxx-driver;c:\mongo-c-driver" "../xmax-mainchain"

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

<a name="buildmongodbcdriver"></a>
## 1. Build mongo-c-driver

Copy `mongo-c-driver` from `libraries` directory to anywhere you want to compile mongo-c-driver. Then execute:
```bash
cd mongo-c-driver
mkdir build
cd build
cmake -G "Visual Studio 15 2017 Win64" "-DCMAKE_INSTALL_PREFIX=c:\mongo-c-driver" "-DCMAKE_PREFIX_PATH=c:\mongo-c-driver" "-DCMAKE_BUILD_TYPE=Release" "-DENABLE_STATIC=ON" ../
```
`C:\mongo-c-driver` is default build location and you can change to wherever you want.
Open `Developer Command Prompt for 2017` and goto `mongo-c-driver/build` then execute:
```bash
msbuild.exe /p:Configuration=Release ALL_BUILD.vcxproj
msbuild.exe /p:Configuration=Release INSTALL.vcxproj
```

<a name="buildmongodbcxxdriver"></a>
## 2. Build mongo-cxx-driver
Copy `mongo-cxx-driver` from `libraries` directory to anywhere you want. Then execute:
```bash
cd mongo-cxx-driver/build
cmake -G "Visual Studio 15 2017 Win64" -DCMAKE_INSTALL_PREFIX=C:\mongo-cxx-driver -DCMAKE_PREFIX_PATH=c:\mongo-c-driver -DBOOST_ROOT=d:\boost_1_67_0 -DBUILD_SHARED_LIBS=OFF -DBSONCXX_POLY_USE_BOOST=1 -DCMAKE_BUILD_TYPE=Release ../
```
You should fill the above directories paths according your situation.
The `C:\mongo-cxx-driver` directory is where you want to install the mongo-cxx-driver. 
The `c:\mongo-c-driver` directory is the building directory for the `mongo-c-driver`. 
The `d:\boost_1_67_0` directory is the boost 1.67 root directory.

Open `Developer Command Prompt for 2017` and goto `mongo-cxx-driver/build` then execute:

```bash
msbuild.exe ALL_BUILD.vcxproj
msbuild.exe INSTALL.vcxproj
```
<a name="buildmongodbproject"></a>
## 3. Use MongoDB in the xmax-mainchain project
In order to use the MongoDB libraries, you need to set `mongo-c-driver` and `mongo-cxx-driver` directories in the `CMAKE_PREFIX_PATH` varaible. For example:
```bash
-DCMAKE_PREFIX_PATH="c:/mongo-c-driver;c:/mongo-cxx-driver"
```
The above two path values are same as building location for `mongo-c-driver` and `mongo-cxx-driver`.
Then you can add `find_package` command to find the necessary MongoDB environment.
```bash
find_package(MongoDB)
```
After executing this command ,you can test `MongoDB_FOUND` CMake variable to make sure if succeed.

Then in the project using MongoDB, you can use MongoDB like this:
```bash
target_include_directories( target_name
	PUBLIC "${MongoDB_INCLUDE_DIR}")
target_link_libraries( target_name   
	${MongoDB_LIBRARIES})
```

<a name="buildmongodbserver"></a>
## 4. Install and run MongoDB service in Windows

### 4.1 Install the MongoDB server
Download and install MongoDB server for Windows which could be found at it's offcial website: `https://www.mongodb.com/download-center`.

If the installation fails using the `msi` installation package, you could try it again without checking the `Install Compass` option.

### 4.2 Run the MongoDB server
After successfual installation, you will find the MongoDb program directory.

Then find a custom data directory(such as `d:\mongo_db\data`) and start the service, for example:
```bash
"D:\Program Files\MongoDB\Server\3.6\bin\mongod.exe" --dbpath d:\mongo_db\data
```

Finally, you can access Mongo DB from your program or compatible tools.
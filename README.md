# XMax - Next Generation Decentralized Application Platform
Official XMax main blockchain source code repository.

# Table of contents

1. [Build on Windows 10](build-windows.md)
2. [Use boost in project](#useboostcmake)





<a name="useboostcmake"></a>
# Use boost in CMake target

## Add required components libs
Add the required component to the **BOOST_REUIQRED_COMPONENTS** variable of the **CMakeLists.txt** in the **xmax-mainchain** root directory. For example:

```bash
set(BOOST_REUIQRED_COMPONENTS)
list(APPEND BOOST_REUIQRED_COMPONENTS filesystem)
```




## Call boost cmake macro

In your target project CMakeLists.txt file, add the **TARGET_USE_BOOST()** macro call. For example:
```bash
TARGET_USE_BOOST()

add_executable( test_program main.cpp)
```
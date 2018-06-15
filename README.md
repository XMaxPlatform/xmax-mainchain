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

In your target project CMakeLists.txt file, add the **\${Boost_INCLUDE_DIR}** to the build target's include directory and add the **${Boost_LIBRARIES}** to the link properties. For example:
```bash
target_include_directories( ${current_lib_name} PUBLIC "include"  "${Boost_INCLUDE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries( ${current_lib_name} ${Boost_LIBRARIES})
```
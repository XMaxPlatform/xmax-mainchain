set(current_lib_name vm_plugin)



file (GLOB SOURCE_FILES "*.cpp"  "*.h")

file (GLOB SCRIPT_INTERFACE "script_interface/*.cpp" "script_interface/*.h")

source_group(script_interface FILES ${SCRIPT_INTERFACE})
source_group( src FILES ${SOURCE_FILES} "CMakeLists.txt")

add_library( ${current_lib_name} ${SOURCE_FILES} ${HEADERS} ${SCRIPT_INTERFACE})

target_link_libraries( ${current_lib_name}

)

target_include_directories( ${current_lib_name} PUBLIC "include"  "${Boost_INCLUDE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" )

target_include_directories( ${current_lib_name}  PUBLIC "include" ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/v8/include )

set_target_properties( ${current_lib_name} PROPERTIES PUBLIC_HEADER "${HEADERS}" FOLDER "5.Plugins")
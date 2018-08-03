# Defines XMAX main blockchain library target.
project(Xmax-mainchain)

enable_testing()

cmake_minimum_required( VERSION 3.11.0 )


set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

set(XMAX_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)
message(STATUS "CMAKE_INSTALL_PREFIX:${CMAKE_INSTALL_PREFIX}")

set( CMAKE_CXX_STANDARD 17 )


if( WIN32 )
    include(windows-config)
endif( WIN32 )
#Config boost environment
set(BOOST_REQUIRED_VERSION "1.67")
set(BOOST_REUIQRED_COMPONENTS)
list(APPEND BOOST_REUIQRED_COMPONENTS 
	program_options 
	system
	thread
	date_time 
	regex 
	serialization 
	unit_test_framework
)

if("${BOOST_ROOT}" STREQUAL "")
  set(BOOST_ROOT $ENV{BOOST_ROOT})
endif()


if( WIN32 )
  set(Boost_USE_MULTITHREADED ON)
  set(BOOST_ALL_DYN_LINK OFF)
  set(Boost_USE_STATIC_LIBS ON)
endif(WIN32)


find_package(Boost ${BOOST_REQUIRED_VERSION} REQUIRED COMPONENTS 

${BOOST_REUIQRED_COMPONENTS}
)


find_package(MongoDB)


set(BUILD_SHARED_LIBS OFF)
set(protobuf_MSVC_STATIC_RUNTIME OFF)

# temp disable:  warning C4996: 'std::result_of': warning STL4014.
# by zhongdaohuan
add_definitions(-D_SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING)
add_definitions(-D_SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING)

set(xmax_core_lib_name pro)

set(xmax_app_lib_name application)

set(xmax_run_exe_name xmaxrun)
set(xmax_db_name unitedb)

set(xmax_bcp_lib_name blockchain_plugin)
set(xmax_builder_lib_name blockbuilder_plugin)
set(xmax_net_lib_name xmaxnet_plugin)
set(xmax_mongo_lib_name mongodb_plugin)
set(contract_util_lib_name contractutil_plugin)
set(xmax_apirpc_lib_name apirpc_plugin)

# Add build directories
add_subdirectory(libraries)
add_subdirectory(foundation)
add_subdirectory(plugins)
add_subdirectory(launch)
add_subdirectory(network)
add_subdirectory(test)


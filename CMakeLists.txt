# Defines XMAX main blockchain library target.
project(Xmax-mainchain)

cmake_minimum_required( VERSION 3.11.0 )


set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

set(XMAX_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

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
)

#if("${BOOST_ROOT}" STREQUAL "")
#  set(BOOST_ROOT $ENV{BOOST_ROOT})
#endif()

message( STATUS "BOOST_ROOT111:${BOOST_ROOT}")


if( WIN32 )
  set(Boost_USE_MULTITHREADED ON)
  set(BOOST_ALL_DYN_LINK OFF)
  set(Boost_USE_STATIC_LIBS ON)
endif(WIN32)


find_package(Boost ${BOOST_REQUIRED_VERSION} REQUIRED COMPONENTS 

${BOOST_REUIQRED_COMPONENTS}
)


message( STATUS "BOOST_ROOT:${BOOST_ROOT}")
message( STATUS "Boost_INCLUDE_DIR:${Boost_INCLUDE_DIR}")


# temp disable:  warning C4996: 'std::result_of': warning STL4014.
# by zhongdaohuan
add_definitions(-D_SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING)
add_definitions(-D_SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING)

set(xmax_core_lib_name core)

set(xmax_app_lib_name application)

set(xmax_run_exe_name xmaxrun)

set(xmax_bcp_lib_name blockchain_plugin)
set(xmax_builder_lib_name blockbuilder_plugin)

# Add build directories
add_subdirectory(libraries)
add_subdirectory(foundation)
add_subdirectory(plugins)
add_subdirectory(launch)

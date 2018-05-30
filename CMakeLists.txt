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
set(BOOST_REQUIRED_VERSION "1.66")
set(BOOST_REUIQRED_COMPONENTS)
list(APPEND BOOST_REUIQRED_COMPONENTS 
	program_options 
	system
	thread
	date_time 
	regex 
	serialization 
)


IF( WIN32 )
  SET(BOOST_ROOT $ENV{BOOST_ROOT})
  set(Boost_USE_MULTITHREADED ON)
  set(BOOST_ALL_DYN_LINK OFF)
ENDIF(WIN32)

find_package(Boost ${BOOST_REQUIRED_VERSION} REQUIRED COMPONENTS ${BOOST_REUIQRED_COMPONENTS})


#Call this macro if building target needs boost
macro(TARGET_USE_BOOST)
    if(Boost_FOUND)
        include_directories(${Boost_INCLUDE_DIRS}) 
        link_directories (${Boost_LIBRARY_DIRS}) 
    else()
        message(FATAL_ERROR "Boost ${BOOST_REQUIRED_VERSION} is not found!")
    endif()
endmacro()


# disable  warning C4996: 'std::result_of': warning STL4014.
add_definitions(-D_SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING)
add_definitions(-D_SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING)

set(xmax_core_lib_name core)
set(xmax_app_lib_name application)

set(xmax_run_exe_name xmaxrun)

set(xmax_bcp_lib_name blockchain_plugin)

add_subdirectory( foundation )
add_subdirectory( plugins )
add_subdirectory( launch )


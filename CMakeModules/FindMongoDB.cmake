# Find mongo db libraries and directories
#
# Usage
#
#     find_package(MongoDB)
#
# In order to find the libraries, you need to pass these variables:
#
#   MONGO_DB_C_ROOT: Set this variable to the ROOT dir of the mongo-c-driver
#
#   MONGO_DB_CXX_ROOT: Set this variable to the ROOT dir of the mongo-cxx-driver
#
#
# Variables output:
#
#  MongoDB_FOUND                 If found all the prerequisites
#  MongoDB_LIBRARIES             Libraries of the mongo db
#  MongoDB_INCLUDE_DIR           Include directories of the mongo db

find_package(libbson-1.0
HINTS "${MONGO_DB_C_ROOT}")

find_package(libmongoc-1.0 1.8
HINTS "${MONGO_DB_C_ROOT}")

if(libmongoc-1.0_FOUND)
    find_package(libmongocxx-static REQUIRED HINTS "${MONGO_DB_CXX_ROOT}")
    
    if((LIBMONGOCXX_VERSION_MAJOR LESS 3) OR ((LIBMONGOCXX_VERSION_MAJOR EQUAL 3) AND (LIBMONGOCXX_VERSION_MINOR LESS 2)))
         find_library(LIBMONGOCXX_STATIC_LIBRARIES
          PATHS ${LIBMONGOCXX_LIBRARY_DIRS} NO_DEFAULT_PATH)   
    endif()   
        
else()
    message(WARNING "Can NOT find libmongoc-1.0.")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MongoDB
    DEFAULT_MSG
    libmongoc-1.0_FOUND
    LIBMONGOCXX_STATIC_INCLUDE_DIRS
    LIBMONGOCXX_STATIC_LIBRARIES
    )

if(MongoDB_FOUND)
    set(MongoDB_INCLUDE_DIR 
    ${LIBMONGOCXX_STATIC_INCLUDE_DIRS}
    )
    
    set(MongoDB_LIBRARIES 
    ${LIBMONGOCXX_STATIC_LIBRARIES}
    )

    message(STATUS "LIBMONGOCXX_STATIC_INCLUDE_DIRS: ${LIBMONGOCXX_STATIC_INCLUDE_DIRS}")

    message(STATUS "MongoDB_INCLUDE_DIR: ${MongoDB_INCLUDE_DIR}")
    message(STATUS "MongoDB_LIBRARIES: ${MongoDB_LIBRARIES}")
    
endif()


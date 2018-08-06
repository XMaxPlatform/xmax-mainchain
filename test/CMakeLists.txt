add_subdirectory( script_test )
add_subdirectory( pro_test)
add_subdirectory( unitedb_test )

if(MongoDB_FOUND)
    add_subdirectory( db_test )
endif()  
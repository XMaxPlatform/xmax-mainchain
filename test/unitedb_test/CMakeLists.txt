set(current_lib_name unitedb_test)

file (GLOB SOURCE_FILES "*.cpp")


source_group( 
	src
	FILES 
	${SOURCE_FILES}
	"CMakeLists.txt"
)

add_executable( ${current_lib_name} ${SOURCE_FILES} )

target_include_directories( ${current_lib_name} PUBLIC
                            ${CMAKE_CURRENT_SOURCE_DIR}/include
                            ${Boost_INCLUDE_DIR})

target_link_libraries( ${current_lib_name} 
                   ${xmax_core_lib_name} 
					${xmax_db_name}
                   ${Boost_LIBRARIES})


set_target_properties( ${current_lib_name} PROPERTIES FOLDER "9.test")

add_test(NAME ${current_lib_name} COMMAND ${current_lib_name})

install( TARGETS 
${current_lib_name}

RUNTIME DESTINATION bin 
LIBRARY DESTINATION lib 
ARCHIVE DESTINATION lib 
)
set(current_lib_name testscript)

file (GLOB SOURCE_FILES "*.cpp")


source_group( 
	src
	FILES 
	${SOURCE_FILES}
	"CMakeLists.txt"
)

add_executable( ${current_lib_name} ${SOURCE_FILES} )

target_include_directories(${current_lib_name} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries( ${current_lib_name}
        PRIVATE ${xmax_app_lib_name} ${xmax_core_lib_name} ${xmax_bcp_lib_name} ${xmax_builder_lib_name}  vm_plugin  
		PRIVATE ${Boost_LIBRARIES}
 )

set_target_properties( ${current_lib_name} PROPERTIES PUBLIC_HEADER "${HEADERS}" FOLDER "9.test")

install( TARGETS 
${current_lib_name}

RUNTIME DESTINATION bin 
LIBRARY DESTINATION lib 
ARCHIVE DESTINATION lib 
)
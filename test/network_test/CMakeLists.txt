set(current_lib_name network_test)

file (GLOB SOURCE_FILES "*.cpp")
file (GLOB HEADERS "include/*.hpp")

source_group( 
	src
	FILES 
	${SOURCE_FILES}
	"CMakeLists.txt"
)

source_group(
    include
    FILES
    ${HEADERS}
)

add_executable( ${current_lib_name} ${SOURCE_FILES} ${HEADERS})

target_include_directories(${current_lib_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries( ${current_lib_name}
        PRIVATE ${xmax_net_lib_name} ${xmax_core_lib_name}  
		PRIVATE ${Boost_LIBRARIES}
 )

set_target_properties( ${current_lib_name} PROPERTIES PUBLIC_HEADER "${HEADERS}" FOLDER "9.test")

install( TARGETS 
${current_lib_name}

RUNTIME DESTINATION bin 
LIBRARY DESTINATION lib 
ARCHIVE DESTINATION lib 
)
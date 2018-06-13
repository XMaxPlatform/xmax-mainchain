set(current_lib_name ${xmax_app_lib_name})


file(GLOB HEADERS "include/*.hpp")
file (GLOB SOURCE_FILES "*.cpp")


source_group( 
	include
	FILES 
	${HEADERS}
)

source_group( 
	src
	FILES 
	${SOURCE_FILES}
)

add_library( ${current_lib_name} ${SOURCE_FILES} ${HEADERS} )

target_link_libraries( ${current_lib_name}
	${Boost_LIBRARIES}
	${xmax_core_lib_name}
)

target_include_directories( ${current_lib_name} PUBLIC "include"  "${Boost_INCLUDE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" )

set_target_properties( ${current_lib_name} PROPERTIES PUBLIC_HEADER "${HEADERS}" FOLDER "1.Foundation")






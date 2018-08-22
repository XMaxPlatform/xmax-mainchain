set(current_lib_name xmaxchain)



file(GLOB HEADERS "include/*.hpp" "include/objects/*.hpp")
file (GLOB SOURCE_FILES "*.cpp" "objects/*.cpp")

source_group( 
	include
	FILES 
	${HEADERS}
)

source_group( 
	src
	FILES 
	${SOURCE_FILES}
	"CMakeLists.txt"
)

add_library( ${current_lib_name} ${SOURCE_FILES} ${HEADERS}  )


target_link_libraries(
        ${current_lib_name} 
        ${Boost_LIBRARIES} ${PLATFORM_LIBRARIES} )
target_include_directories(${current_lib_name}
        ${current_lib_name} PUBLIC "include" "${CMAKE_CURRENT_SOURCE_DIR}"
        ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/picosha2
        
)



set_target_properties( ${current_lib_name} PROPERTIES PUBLIC_HEADER "${HEADERS}" FOLDER "4.BlockChain")

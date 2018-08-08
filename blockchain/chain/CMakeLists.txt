file(GLOB HEADERS "include/*.hpp" "include/objects/*.hpp")
file (GLOB SOURCE_FILES "*.cpp" "objects/*.cpp")



add_library( xmaxchain ${SOURCE_FILES} ${HEADERS}  )


target_link_libraries(
        xmaxchain 
        ${Boost_LIBRARIES} ${PLATFORM_LIBRARIES} )
target_include_directories(
        xmaxchain PUBLIC "include"
        ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR}/include
        
)



set_target_properties( xmaxchain PROPERTIES PUBLIC_HEADER "${HEADERS}" FOLDER "4.BlockChain")

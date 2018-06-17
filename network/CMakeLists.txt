


set(PROTO_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/protos)
set(PROTO_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/protos_cpp)
set(PROTO_LIB_SRC_DIR ${CMAKE_SOURCE_DIR}/libraries/protobuf/src)

file (GLOB ProtoFiles "${PROTO_SOURCE_DIR}/*.proto")


set(PROTO_COMPILE_TARGETS)
set(PROTO_COMPILE_TARGETS_SOURCES)
set(PROTO_COMPILE_TARGETS_HEADERS)

macro(add_protobuf_compile_target proto_file_name)
    list(APPEND PROTO_COMPILE_TARGETS
        ${PROTO_SOURCE_DIR}/${proto_file_name}.proto
    )

    list(APPEND PROTO_COMPILE_TARGETS_HEADERS 
        ${PROTO_BUILD_DIR}/${proto_file_name}.pb.h 
    )

    list(APPEND PROTO_COMPILE_TARGETS_SOURCES 
        ${PROTO_BUILD_DIR}/${proto_file_name}.pb.cc
    )


    add_custom_command(OUTPUT  ${PROTO_SOURCE_DIR}/${proto_file_name}.pb.cc       
        COMMAND $<TARGET_FILE:protoc> -I=${PROTO_SOURCE_DIR} --proto_path=${PROTO_LIB_SRC_DIR} --cpp_out=${PROTO_BUILD_DIR} ${PROTO_SOURCE_DIR}/${proto_file_name}.proto
    )
endmacro()


#Add compile proto files list here
# foreach(proto_file_fullpath ${ProtoFiles})        
#     get_filename_component(proto_file_name ${proto_file_fullpath} NAME_WE)

#     message(STATUS "Found proto-buf file: ${proto_file_fullpath}")
#     add_protobuf_compile_target(${proto_file_name})
# endforeach()

#Add compile proto manually
add_protobuf_compile_target(test)
#add_protobuf_compile_target(timestamp)

add_custom_target(compile_protos
    DEPENDS protoc ${PROTO_COMPILE_TARGETS_SOURCES}
    COMMAND echo "Generate proto targets ${PROTO_COMPILE_TARGETS_SOURCES}."
    SOURCES ${PROTO_COMPILE_TARGETS}
)

set_target_properties(compile_protos
    PROPERTIES
        FOLDER "Tools"
)

add_subdirectory(test)
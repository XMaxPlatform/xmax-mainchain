


set(PROTO_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/protos)
set(PROTO_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/protos_cpp)
set(PROTO_LIB_SRC_DIR ${CMAKE_SOURCE_DIR}/libraries/protobuf/src)

file (GLOB ProtoFiles "${PROTO_SOURCE_DIR}/*.proto")

message(STATUS "PROTO_LIB_SRC_DIR:${PROTO_LIB_SRC_DIR}")

set(PROTO_COMPILE_TARGETS)

macro(add_protobuf_compile_target proto_file_name)
    list(APPEND PROTO_COMPILE_TARGETS 
        ${PROTO_SOURCE_DIR}/${proto_file_name} 
    )
    add_custom_command(OUTPUT  ${PROTO_SOURCE_DIR}/${proto_file_name}        
        COMMAND $<TARGET_FILE:protoc> -I=${PROTO_SOURCE_DIR} --proto_path=${PROTO_LIB_SRC_DIR} --cpp_out=${PROTO_BUILD_DIR} ${PROTO_SOURCE_DIR}/${proto_file_name}
    )
endmacro()

# Add compile proto files list here
# foreach(proto_file ${ProtoFiles})
#     message(STATUS "Found proto-buf file: ${proto_file}")
#     add_protobuf_compile_target(test.proto)
# endforeach()

add_protobuf_compile_target(test.proto)

add_custom_target(compile_protos
    DEPENDS protoc ${PROTO_COMPILE_TARGETS}
    COMMAND echo "Generate proto targets ${PROTO_COMPILE_TARGETS}."
    
)

set_target_properties(compile_protos
    PROPERTIES
        FOLDER "Tools"
)
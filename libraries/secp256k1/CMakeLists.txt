project(secp256k1 LANGUAGES C)

file(GLOB SECP256K1_INCLUDE_HEADERS "include/*.h")

set(COMMON_COMPILE_FLAGS ENABLE_MODULE_RECOVERY ENABLE_MODULE_ECDH USE_ECMULT_STATIC_PRECOMPUTATION USE_FIELD_INV_BUILTIN USE_NUM_NONE USE_SCALAR_INV_BUILTIN)
if (MSVC)
    set(COMPILE_FLAGS USE_FIELD_10X26 USE_SCALAR_8X32)
    set(COMPILE_OPTIONS "")
else()
    set(COMPILE_FLAGS USE_FIELD_5X52 USE_SCALAR_4X64 HAVE_BUILTIN_EXPECT HAVE___INT128)
    set(COMPILE_OPTIONS -O3 -W -std=c89 -pedantic -Wall -Wextra -Wcast-align -Wnested-externs -Wshadow -Wstrict-prototypes -Wno-unused-function -Wno-long-long -Wno-overlength-strings -fvisibility=hidden)
endif()

add_executable(gen_context src/gen_context.c)
target_include_directories(gen_context PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(ecmult_static_context gen_context WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_library(secp256k1 STATIC src/secp256k1.c ${SECP256K1_INCLUDE_HEADERS})
target_compile_definitions(secp256k1 PRIVATE ${COMMON_COMPILE_FLAGS} ${COMPILE_FLAGS})
target_include_directories(secp256k1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_options(secp256k1 PRIVATE ${COMPILE_OPTIONS})
add_dependencies(secp256k1 ecmult_static_context)

# Add targets to folder
set_target_properties(secp256k1 PROPERTIES FOLDER "libraries/secp256k1")
set_target_properties(ecmult_static_context PROPERTIES FOLDER "libraries/secp256k1")
set_target_properties(gen_context PROPERTIES FOLDER "libraries/secp256k1")


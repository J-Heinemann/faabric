cmake_minimum_required(VERSION 3.13.0)
project(faabricmpi)

# -----------------------------------------------
# This library must support being compiled on its own
# -----------------------------------------------

set(FAABRIC_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../include)

include_directories(${FAABRIC_INCLUDE_DIR})

set(PUBLIC_HEADERS
        ${FAABRIC_INCLUDE_DIR}/faabric/mpi/mpi.h
    )

add_library(faabricmpi STATIC 
        mpi.cpp
        ${PUBLIC_HEADERS}
    )

set_target_properties(faabricmpi 
    PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}"
    )

if(FAASM_BUILD_TYPE STREQUAL "wasm")
    set(LIB_DIRECTORY ${CMAKE_SYSROOT}/lib/wasm32-wasi)
else()
    set(LIB_DIRECTORY ${CMAKE_SYSROOT}/lib)
endif()

install(TARGETS faabricmpi
    ARCHIVE DESTINATION ${LIB_DIRECTORY}
    LIBRARY DESTINATION ${LIB_DIRECTORY}
    PUBLIC_HEADER DESTINATION ${CMAKE_SYSROOT}/include/faabric/mpi
)


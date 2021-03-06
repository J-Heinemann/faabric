set(OUTPUT_FILES
        ${CMAKE_CURRENT_BINARY_DIR}/faabric.pb.cc
        ${CMAKE_CURRENT_BINARY_DIR}/faabric.pb.h
        ${CMAKE_CURRENT_BINARY_DIR}/faabric.grpc.pb.cc
        ${CMAKE_CURRENT_BINARY_DIR}/faabric.grpc.pb.h
        )

add_custom_command(
        OUTPUT ${OUTPUT_FILES}
        COMMAND protoc
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${CMAKE_CURRENT_LIST_DIR}"
        --plugin=protoc-gen-grpc=${GRPC_PLUGIN}
        faabric.proto
        DEPENDS faabric.proto
)

include_directories(${FAABRIC_INCLUDE_DIR}/faabric/proto/)

file(GLOB HEADERS "${FAABRIC_INCLUDE_DIR}/faabric/proto/*.h")

set(LIB_FILES
        RPCServer.cpp
        ${HEADERS}
        ${OUTPUT_FILES}
        )

if(FAABRIC_STATIC_LIBS)
    faabric_private_lib(proto "${LIB_FILES}")
else()
    faabric_public_lib(proto "${LIB_FILES}")
endif()

target_link_libraries(proto
        ${Protobuf_LIBRARIES}
        gRPC::grpc++
        gRPC::grpc++_reflection
        )

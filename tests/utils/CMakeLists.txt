include_directories(${CMAKE_CURRENT_LIST_DIR})

set(LIB_FILES
    exec_graph_utils.cpp
    message_utils.cpp
    system_utils.cpp
    faabric_utils.h
    )

faabric_private_lib(faabric_test_utils "${LIB_FILES}")

target_link_libraries(faabric_test_utils
    catch
    faabric
)

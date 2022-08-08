set(SRC_LIST
    mprpc_application.cc
    mprpc_config.cc
    mprpc_provider.cc
    rpc_header.pb.cc
    mprpc_channel.cc
    mprpc_controller.cc
    zookeeper_util.cc
    )
aux_source_directory(mprpc SRC_LIST)
add_library(mprpc ${SRC_LIST})
target_link_libraries(mprpc muduo_net muduo_base pthread zookeeper_mt)
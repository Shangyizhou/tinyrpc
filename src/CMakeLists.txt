set(SRC_LIST
    MprpcApplication.cc
    MprpcProvider.cc
    rpc_header.pb.cc
    MprpcChannel.cc
    MprpcController.cc
    ZookeeperUtil.cc
    )
aux_source_directory(mprpc SRC_LIST)
# 生成静态库
add_library(mprpc ${SRC_LIST})
# 目标需要链接的库
target_link_libraries(mprpc muduo_net muduo_base pthread zookeeper_mt)
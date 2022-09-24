set(SRC_LIST 
    call_user_service.cc 
    ../user.pb.cc
    )

add_executable(consumer ${SRC_LIST})
target_link_libraries(consumer mprpc protobuf)
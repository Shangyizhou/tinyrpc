set(CLIENT_LIST client.cc ../user.pb.cc)
set(SERVER_LIST server.cc ../user.pb.cc)

add_executable(client ${CLIENT_LIST})
add_executable(server ${SERVER_LIST})

target_link_libraries(client mprpc protobuf)
target_link_libraries(server mprpc protobuf)
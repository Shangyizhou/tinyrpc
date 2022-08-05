#include <iostream>
#include "mprpc_application.h"
#include "user.pb.h"
#include "mprpc_channel.h"

int main(int argc, char **argv)
{
    MprpcApplication::Init(argc, argv);

    // 调用远程发布的rpc方法Login
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());
    fixbug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");
    // rpc方法的响应
    fixbug::LoginResponse response;
    printf("this is a test\n");
    // 发起rpc方法的调用
    stub.Login(nullptr, &request, &response, nullptr);

    if (0 == response.result().errcode())
    {
        std::cout << "rpc Login response success:" << response.success() << std::endl;
    }
    else
    {
        std::cout << "rpc response error : " << response.result().errmsg() << std::endl;
    }

    return 0;
}
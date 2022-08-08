#include <iostream>
#include "mprpc_application.h"
#include "user.pb.h"
#include "mprpc_channel.h"
#include "mprpc_controller.h"

void loginService(fixbug::UserServiceRpc_Stub& stub, MprpcController& controller)
{
    fixbug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");
    // rpc方法的响应
    fixbug::LoginResponse response;

    // 发起rpc方法的调用
    stub.Login(&controller, &request, &response, nullptr);

    if (controller.Failed())
    {
        std::cout << controller.ErrorText() << std::endl;
    }
    else
    {
        if (0 == response.result().errcode())
        {
            std::cout << "rpc Login response success:" << response.success() << std::endl;
        }
        else
        {
            std::cout << "rpc response error : " << response.result().errmsg() << std::endl;
        }
    }
}

void registerService(fixbug::UserServiceRpc_Stub& stub, MprpcController& controller)
{
    // 演示调用Register
    fixbug::RegisterRequest req;
    // TODO:整数形式是否可行
    req.set_id(100);
    req.set_name("mprpc");
    req.set_pwd("123456");
    fixbug::RegisterResponse rsp;

    // 以同步的方式发起rpc调用请求，等待返回结果
    stub.Register(&controller, &req, &rsp, nullptr);
    if (controller.Failed())
    {
        std::cout << controller.ErrorText() << std::endl;
    }
    else
    {
        if (0 == rsp.result().errcode())
        {
            std::cout << "rpc Register response success:" << rsp.success() << std::endl;
        }
        else
        {
            std::cout << "rpc response error : " << rsp.result().errmsg() << std::endl;
        }
    }
}

int main(int argc, char **argv)
{
    MprpcApplication::Init(argc, argv);

    // 调用远程发布的rpc方法Login
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());
    MprpcController controller;

    loginService(stub, controller);
    registerService(stub, controller);

    return 0;
}
#ifndef __MPRPC_CHANNEL_H__
#define __MPRPC_CHANNEL_H__

#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <muduo/base/Logging.h>
#include "MprpcApplication.h"
#include "ErrorCode.h"

#include <string.h>

struct ServiceAddress
{
    std::string ip;
    uint16_t port;
};


class MprpcChannel : public google::protobuf::RpcChannel
{
public:
    // 重写RpcChannel::CallMethod方法，统一做rpc方法的序列化和网络发送
    void CallMethod(const google::protobuf::MethodDescriptor* method,
                          google::protobuf::RpcController* controller, 
                          const google::protobuf::Message* request,
                          google::protobuf::Message* response, 
                          google::protobuf::Closure* done);
private:
    // 按照自定义协议打包请求信息，成功返回序列化后的字符串
    RPC_CHANNEL_CODE PackageRequest(std::string* rpc_request_str,
                                    const google::protobuf::MethodDescriptor* method,
                                    google::protobuf::RpcController* controller,
                                    const google::protobuf::Message* request);
    // 通过网络发送打包好的请求信息
    RPC_CHANNEL_CODE SendRpcRquest(int* fd,
                                   const ServiceAddress& service_address, 
                                   const std::string& send_rpc_str,
                                   ::google::protobuf::RpcController* controller);
    // 接收对端的响应
    RPC_CHANNEL_CODE ReceiveRpcResponse(const int& client_fd,
                                        google::protobuf::Message* response,
                                        ::google::protobuf::RpcController* controller);
};

#endif // __MPRPC_CHANNEL_H__
#ifndef __MPRPC_CHANNEL_H__
#define __MPRPC_CHANNEL_H__

#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include "mprpc_application.h"
#include "mprpc_controller.h"

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
};

#endif // __MPRPC_CHANNEL_H__
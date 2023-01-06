#include "MprpcChannel.h"
#include "MprpcController.h"
#include "rpc_header.pb.h"

#include <muduo/net/TcpClient.h>

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>

// 读取配置文件rpcserver的信息
// std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
// uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
// rpc调用方想调用service_name的method_name服务，需要查询zk上该服务所在的host信息
bool GetServiceAddress(const std::string& service_name, 
                    const std::string& method_name, 
                    ServiceAddress& service_address,
                    ::google::protobuf::RpcController* controller)
{
    ZkClient zkCli;
    zkCli.Start();
    std::string method_path = "/" + service_name + "/" + method_name;
    // 127.0.0.1:8000
    std::string host_data = zkCli.GetData(method_path.c_str());
    if (host_data == "")
    {
        controller->SetFailed(method_path + " is not exist!");
        return false;
    }
    int idx = host_data.find(":");
    if (idx == -1)
    {
        controller->SetFailed(method_path + " address is invalid!");
        return false;
    }

    service_address.ip = host_data.substr(0, idx);
    service_address.port = atoi(host_data.substr(idx+1, host_data.size()-idx).c_str()); 
    return true;
}

RPC_CHANNEL_CODE MprpcChannel::PackageRequest(std::string* rpc_request_str,
                                              const google::protobuf::MethodDescriptor* method,
                                              google::protobuf::RpcController* controller,
                                              const google::protobuf::Message* request)
{
    std::string service_name = method->service()->name();
    std::string method_name = method->name();

    // request里面储存着请求参数
    // 我们需要将请求参数序列化
    uint32_t args_size = 0;
    std::string args_str;
    if (request->SerializeToString(&args_str)) {
        // uint32_t args_size = 0;
        args_size = args_str.size();
    } else {
        controller->SetFailed("serialize request error!");
        return CHANNEL_PACKAGE_ERR;
    }

    // 定义rpc的请求header
    mprpc::RpcHeader rpc_header;
    rpc_header.set_service_name(service_name);
    rpc_header.set_methon_name(method_name);
    rpc_header.set_args_size(args_size);

    // 获取请求header的字符串长度
    uint32_t header_size = 0;
    std::string rpc_header_str;
    if (rpc_header.SerializeToString(&rpc_header_str)) {
        header_size = rpc_header_str.size();
    } else {
        controller->SetFailed("serialize rpc header error!");
        return CHANNEL_PACKAGE_ERR;
    }

    // + ----------- +------+ ------------ +------+ ----------- +------+ --------- +------+ -------- +
    // + header_size +------+ service_name +------+ method_name +------+ args_size +------+ args_str +
    // + ----------- +------+ ------------ +------+ ----------- +------+ --------- +------+ -------- +
    // memset(rpc_request_str, '\0', sizeof(rpc_request_str));
    (*rpc_request_str).insert(0, std::string((char*)&header_size, 4));
    (*rpc_request_str) += rpc_header_str;
    (*rpc_request_str) += args_str;

    return CHANNEL_SUCCESS;
}

// 通过网络发送打包好的请求信息
RPC_CHANNEL_CODE MprpcChannel::SendRpcRquest(int* fd,
                               const ServiceAddress& service_address, 
                               const std::string& send_rpc_str,
                               ::google::protobuf::RpcController* controller)
{
    // 使用tcp编程，完成rpc方法的远程调用
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == client_fd)
    {
        char errtxt[512] = {0};
        sprintf(errtxt, "create socket error! errno:%d", errno);
        controller->SetFailed(errtxt);
        return CHANNEL_SEND_ERR;
    }
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(service_address.port);
    server_addr.sin_addr.s_addr = inet_addr(service_address.ip.c_str());

    // 连接rpc服务节点
    if (-1 == connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)))
    {
        close(client_fd);
        char errtxt[512] = {0};
        sprintf(errtxt, "connect error! errno:%d", errno);
        controller->SetFailed(errtxt);
        return CHANNEL_SEND_ERR;
    }

    // 发送rpc请求
    if (-1 == send(client_fd, send_rpc_str.c_str(), send_rpc_str.size(), 0))
    {
        close(client_fd);
        char errtxt[512] = {0};
        sprintf(errtxt, "send error! errno:%d", errno);
        controller->SetFailed(errtxt);
        return CHANNEL_SEND_ERR;
    }
    (*fd) = client_fd;
    return CHANNEL_SUCCESS;
}

RPC_CHANNEL_CODE MprpcChannel::ReceiveRpcResponse(const int& client_fd,
                        google::protobuf::Message* response,
                        ::google::protobuf::RpcController* controller)
{
    // 接收响应缓冲区
    char recv_buf[1024] = {0};
    int recv_size = 0;
    if (-1 == (recv_size = recv(client_fd, recv_buf, 1024, 0)))
    {
        close(client_fd);
        char errtxt[512] = {0};
        sprintf(errtxt, "recv error! errno:%d", errno);
        controller->SetFailed(errtxt);
        return CHANNEL_RECEIVE_ERR;
    }

    std::string response_str(recv_buf, 0, recv_size);
    // 序列化响应信息
    if (!response->ParseFromArray(recv_buf, recv_size)) // TODO:使用Array而不是String
    {
        close(client_fd);
        char errtxt[512] = {0};
        sprintf(errtxt, "parse error! response_str:%s", recv_buf);
        controller->SetFailed(errtxt);
        return CHANNEL_RECEIVE_ERR;
    }

    close(client_fd);    
    return CHANNEL_SUCCESS;
}

// 重写RpcChannel::CallMethod方法，统一做rpc方法的序列化和网络发送
// header_size service_name method_name args_size args
void MprpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                          google::protobuf::RpcController* controller, 
                          const google::protobuf::Message* request,
                          google::protobuf::Message* response, 
                          google::protobuf::Closure* done)
{
    // 按照自定义协议封装RPC请求
    std::string rpc_request_str;
    std::string* ptr = &rpc_request_str;
    RPC_CHANNEL_CODE res =  PackageRequest(ptr, method, controller, request);
    if (res) {
        LOG_ERROR << "PackageRequest failed()";
        return;
    }

    // 获取微服务的地址
    ServiceAddress service_address;
    if (false == GetServiceAddress(method->service()->name(), method->name(), service_address, controller))
    {
        LOG_ERROR << "GetServiceAddress failed()";
        return;
    }

    // 通过网络发送RPC请求，返回clientfd，我们将用此接收响应
    int client_fd;
    res = SendRpcRquest(&client_fd, service_address, rpc_request_str, controller);
    if (res) {
        LOG_ERROR << "SendRpcRquest failed()";
        return;
    }
    
    // 接收响应信息
    res = ReceiveRpcResponse(client_fd, response, controller);
    if (res) {
        LOG_ERROR << "ReceiveRpcResponse failed()";
        return;
    }
}
#include <muduo/net/TcpClient.h>
#include <string>
#include "mprpc_channel.h"
#include "mprpc_controller.h"
#include "rpc_header.pb.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>

struct ServiceAddress
{
    std::string ip;
    uint16_t port;
};

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

bool sendRpcRquest(const ServiceAddress& service_address)
{

}

// 重写RpcChannel::CallMethod方法，统一做rpc方法的序列化和网络发送
// header_size service_name method_name args_size args
void MprpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                          google::protobuf::RpcController* controller, 
                          const google::protobuf::Message* request,
                          google::protobuf::Message* response, 
                          google::protobuf::Closure* done)
{
    // 通过ServiceDescriptor获取调用rpc服务名称，方法名称
    const ::google::protobuf::ServiceDescriptor* sd = method->service();    
    std::string service_name = sd->name();
    std::string method_name = method->name();

    // 将请求参数序列化并获取字符串长度
    uint32_t args_size = 0;
    std::string args_str;
    if (request->SerializeToString(&args_str))
    {
        uint32_t args_size = 0;
        args_size = args_str.size();
    }
    else
    {
        controller->SetFailed("serialize request error!");
        return;
    }

    // 定义rpc的请求header
    mprpc::RpcHeader rpcHeader;
    rpcHeader.set_service_name(service_name);
    rpcHeader.set_methon_name(method_name);
    rpcHeader.set_args_size(args_size);

    // 获取请求header的字符串长度
    uint32_t header_size = 0;
    std::string rpc_header_str;
    if (rpcHeader.SerializeToString(&rpc_header_str))
    {
        header_size = rpc_header_str.size();
    }
    else
    {
        controller->SetFailed("serialize rpc header error!");
        return;
    }

    std::string send_rpc_str;
    send_rpc_str.insert(0, std::string((char*)&header_size, 4));
    send_rpc_str += rpc_header_str;
    send_rpc_str += args_str;

    // 打印调试信息
    std::cout << "============================================" << std::endl;
    std::cout << "header_size: " << header_size << std::endl; 
    std::cout << "rpc_header_str: " << rpc_header_str << std::endl; 
    std::cout << "service_name: " << service_name << std::endl; 
    std::cout << "method_name: " << method_name << std::endl; 
    std::cout << "args_str: " << args_str << std::endl; 
    std::cout << "============================================" << std::endl;

    // 获取微服务的地址
    ServiceAddress service_address;
    if (false == GetServiceAddress(service_name, method_name, service_address, controller))
    {
        return;
    }

    // 使用tcp编程，完成rpc方法的远程调用
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == clientfd)
    {
        char errtxt[512] = {0};
        sprintf(errtxt, "create socket error! errno:%d", errno);
        controller->SetFailed(errtxt);
        return;
    }
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(service_address.port);
    server_addr.sin_addr.s_addr = inet_addr(service_address.ip.c_str());

    // 连接rpc服务节点
    if (-1 == connect(clientfd, (struct sockaddr*)&server_addr, sizeof(server_addr)))
    {
        close(clientfd);
        char errtxt[512] = {0};
        sprintf(errtxt, "connect error! errno:%d", errno);
        controller->SetFailed(errtxt);
        return;
    }

    // 发送rpc请求
    if (-1 == send(clientfd, send_rpc_str.c_str(), send_rpc_str.size(), 0))
    {
        close(clientfd);
        char errtxt[512] = {0};
        sprintf(errtxt, "send error! errno:%d", errno);
        controller->SetFailed(errtxt);
        return;
    }

    // 接收rpc请求的响应值
    char recv_buf[1024] = {0};
    int recv_size = 0;
    if (-1 == (recv_size = recv(clientfd, recv_buf, 1024, 0)))
    {
        close(clientfd);
        char errtxt[512] = {0};
        sprintf(errtxt, "recv error! errno:%d", errno);
        controller->SetFailed(errtxt);
        return;
    }

    std::string response_str(recv_buf, 0, recv_size);
    if (!response->ParseFromArray(recv_buf, recv_size)) // TODO:使用Array而不是String
    {
        close(clientfd);
        char errtxt[512] = {0};
        sprintf(errtxt, "parse error! response_str:%s", recv_buf);
        controller->SetFailed(errtxt);
        return;
    }

    close(clientfd);
}
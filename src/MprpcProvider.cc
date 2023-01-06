#include "MprpcProvider.h"
#include "rpc_header.pb.h"
#include "google/protobuf/descriptor.h"

#include <functional>

// 框架，需要设计成抽象基类
void MprpcProvider::NotifyService(google::protobuf::Service *service) 
{
    ServiceInfo service_info;

    // 获取服务对象的描述信息
    const google::protobuf::ServiceDescriptor *pserviceDesc = service->GetDescriptor();
    
    // 获取服务的名字
    // 需包含 google/protobuf/descriptor.h 
    std::string service_name = pserviceDesc->name();

    std::cout << "=========================================" << std::endl;
    std::cout << "service_name:" << service_name << std::endl;

    for (int i = 0; i < pserviceDesc->method_count(); ++i)
    {
        // 获取了服务对象指定下标的服务方法的描述「方法名，方法」
        const google::protobuf::MethodDescriptor* pmethodDesc = pserviceDesc->method(i);
        std::string method_name = pmethodDesc->name();
        service_info.m_methodMap.insert({method_name, pmethodDesc});
        std::cout << "method_name:" << method_name << std::endl;
    }
    std::cout << "=========================================" << std::endl;
    service_info.m_service = service;
    m_serviceMap.insert({service_name, service_info});
}

// 将服务注册到zookeeper上
void MprpcProvider::RegisterZookeeper(const muduo::net::InetAddress& address, ZkClient* zkCli)
{
    // 把当前rpc节点上要发布的服务全部注册到zk上面，让rpc client可以从zk上发现服务
    zkCli->Start();
    // service_name为永久性节点，method_name为临时性节点
    for (auto &sp : m_serviceMap) 
    {
        // /UserServiceRpc
        std::string service_path = "/" + sp.first;
        zkCli->Create(service_path.c_str(), nullptr, 0);
        for (auto &mp : sp.second.m_methodMap)
        {
            // path = /UserServiceRpc/Login 
            // value = 127.0.0.1:2181
            std::string method_path = service_path + "/" + mp.first;
            char method_path_data[128] = {0};
            sprintf(method_path_data, "%s:%d", address.toIp().c_str(), 2181);
            // ZOO_EPHEMERAL表示znode是一个临时性节点
            zkCli->Create(method_path.c_str(), method_path_data, strlen(method_path_data), ZOO_EPHEMERAL);
        }
    }
}


// 启动rpc服务节点，开始提供rpc远程网络调用服务
void MprpcProvider::Run() 
{
    // TcpServer绑定此地址
    std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpc_server_ip");
    uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpc_server_port").c_str());
    muduo::net::InetAddress address(ip, port);

    // 创建 TcpServer 对象
    muduo::net::TcpServer server(&m_eventLoop, address, "RpcProvider");

    // 设置muduo库线程数量
    server.setThreadNum(4);

    // 绑定连接回调和消息读写回调方法  分离了网络代码和业务代码
    server.setConnectionCallback(std::bind(&MprpcProvider::OnConnection, this, std::placeholders::_1));
    server.setMessageCallback(std::bind(&MprpcProvider::OnMessage, this, std::placeholders::_1, 
            std::placeholders::_2, std::placeholders::_3));

    // 将服务注册到zookeeper上
    ZkClient zkCli;
    RegisterZookeeper(address, &zkCli);
    
    LOG_INFO << "RpcProvider start service at ip:" << ip << " port:" << port;

    // 启动网络服务
    server.start();
    m_eventLoop.loop();
}

// socket连接回调(新连接到来或者连接关闭事件)
void MprpcProvider::OnConnection(const muduo::net::TcpConnectionPtr &conn)
{
    if (!conn->connected())
    {
        // 和rpc client的连接断开了
        conn->shutdown();
    }
}

// 解析数据包
// 将buffer前四个字节读出来，方便知道接下来要读取多少数据
void MprpcProvider::ParseRequest(muduo::net::Buffer* buffer, RequestInfo* req_info)
{
    // 网络上接受的远程rpc调用请求的字符流
    std::string recv_buf = buffer->retrieveAllAsString();
    
    uint32_t header_size = 0;
    char* address = (char*)&header_size;
    recv_buf.copy(address, 4, 0);

    // copy[4, 4 + header_size) -> rpc_header_str
    std::string rpc_header_str = recv_buf.substr(4, header_size);

    // 数据反序列化
    mprpc::RpcHeader rpcHeader;
    if (!rpcHeader.ParseFromString(rpc_header_str))
    {
        LOG_WARN << "rpc_header_str:" << rpc_header_str << " parse error!";
        return;
    }
    
    req_info->service_name = rpcHeader.service_name();
    req_info->method_name = rpcHeader.methon_name();
    req_info->args_size = rpcHeader.args_size();
    req_info->args_str = recv_buf.substr(4 + header_size, req_info->args_size);
    
    return;
}

/**
 * 如果远程有一个rpc服务的调用请求，那么OnMessage方法就会响应
 * 这个发送的信息都是经过了协议商定的
 * service_name method_name args 定义proto的message类型，进行数据头的序列化和反序列化
 * 既然是针对于数据头，那么我们需要获取指定的一些长度(需要考虑TCP黏包问题)
 * header_size header_str args_str
 */
void MprpcProvider::OnMessage(const muduo::net::TcpConnectionPtr& conn, 
                                muduo::net::Buffer* buffer,
                                muduo::Timestamp timestamp)
{
    // 解析自定义的请求信息
    RequestInfo request_info;
    ParseRequest(buffer, &request_info);

#if 0
    // 打印调试信息
    LOG_INFO << "============================================";
    LOG_INFO << "header_size: " << header_size <<; 
    LOG_INFO << "rpc_header_str: " << rpc_header_str <<;
    LOG_INFO << "service_name: " << service_name <<;
    LOG_INFO << "method_name: " << method_name <<;
    LOG_INFO << "args_str: " << args_str <<;
    LOG_INFO << "============================================";
#endif

    // 在注册表上查找服务
    auto it = m_serviceMap.find(request_info.service_name);
    if (it == m_serviceMap.end())
    {
        LOG_WARN << request_info.service_name << " is not exist!";
        return;
    }

    auto mit = it->second.m_methodMap.find(request_info.method_name);
    if (mit == it->second.m_methodMap.end())
    {
        LOG_WARN << request_info.service_name << ":" << request_info.method_name << " is not exist!";
        return;
    }
    
    // 负责Response
    google::protobuf::Service *service = it->second.m_service; 
    const google::protobuf::MethodDescriptor *method = mit->second; 

    // 生成rpc方法调用的请求request和响应response参数
    // request参数需要单独解析，之前只是知道了request的序列化字符串长度
    google::protobuf::Message *request = service->GetRequestPrototype(method).New();
    if (!request->ParseFromString(request_info.args_str))
    {
        LOG_WARN << "request parse error, content:" << request_info.args_str;
        return;
    }
    google::protobuf::Message *response = service->GetResponsePrototype(method).New();

    // 给下面的method方法的调用，绑定一个Closure的回调函数
    google::protobuf::Closure *done = google::protobuf::NewCallback<MprpcProvider, 
                                                                    const muduo::net::TcpConnectionPtr&, 
                                                                    google::protobuf::Message*>
                                                                    (this, 
                                                                    &MprpcProvider::SendRpcResponse, 
                                                                    conn, response);

    // 在框架上根据远端rpc请求，调用当前rpc节点上发布的方法
    // new UserService().Login(controller, request, response, done)
    service->CallMethod(method, nullptr, request, response, done);
}

// Closure的回调操作，用于序列化rpc的响应和网络发送
void MprpcProvider::SendRpcResponse(const muduo::net::TcpConnectionPtr& conn, 
                                    google::protobuf::Message* response)
{
    std::string response_str;
    if (response->SerializeToString(&response_str))
    {
        // 序列化成功后，通过网络将rpc方法执行结果发送给rpc的调用方
        conn->send(response_str);
    }
    else
    {
        std::cout << "serialize response_str error!" << std::endl;
    }
    // 半关闭
    conn->shutdown();
}
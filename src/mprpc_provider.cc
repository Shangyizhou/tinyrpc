#include "mprpc_provider.h"
#include "google/protobuf/descriptor.h"
#include "rpc_header.pb.h"
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
    // 获取服务对象service的方法的数量
    int methodCnt = pserviceDesc->method_count();

    std::cout << "=========================================" << std::endl;
    std::cout << "service_name:" << service_name << std::endl;

    for (int i=0; i < methodCnt; ++i)
    {
        // 获取了服务对象指定下标的服务方法的描述
        const google::protobuf::MethodDescriptor* pmethodDesc = pserviceDesc->method(i);
        std::string method_name = pmethodDesc->name();
        service_info.m_methodMap.insert({method_name, pmethodDesc});
        std::cout << "method_name:" << method_name << std::endl;
    }
    std::cout << "=========================================" << std::endl;
    service_info.m_service = service;
    m_serviceMap.insert({service_name, service_info});
}

// 启动rpc服务节点，开始提供rpc远程网络调用服务
void MprpcProvider::Run() 
{
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

    // 把当前rpc节点上要发布的服务全部注册到zk上面，让rpc client可以从zk上发现服务
    // session timeout   30s     zkclient 网络I/O线程  1/3 * timeout 时间发送ping消息
    ZkClient zkCli;
    zkCli.Start();
    // service_name为永久性节点    method_name为临时性节点
    for (auto &sp : m_serviceMap) 
    {
        // /service_name   /UserServiceRpc
        std::string service_path = "/" + sp.first;
        zkCli.Create(service_path.c_str(), nullptr, 0);
        for (auto &mp : sp.second.m_methodMap)
        {
            // /service_name/method_name   /UserServiceRpc/Login 存储当前这个rpc服务节点主机的ip和port
            std::string method_path = service_path + "/" + mp.first;
            char method_path_data[128] = {0};
            sprintf(method_path_data, "%s:%d", ip.c_str(), 2181);
            // ZOO_EPHEMERAL表示znode是一个临时性节点
            zkCli.Create(method_path.c_str(), method_path_data, strlen(method_path_data), ZOO_EPHEMERAL);
        }
    }
    
    std::cout << "RpcProvider start service at ip:" << ip << " port:" << port << std::endl;

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

/**
 * 这个发送的信息都是经过了协议商定的
 * service_name method_name args 定义proto的message类型，进行数据头的序列化和反序列化
 * 既然是针对于数据头，那么我们需要获取指定的一些长度(需要考虑TCP黏包问题)
 * header_sezi header_str args_str
 */
// 如果远程有一个rpc服务的调用请求，那么OnMessage方法就会响应
void MprpcProvider::OnMessage(const muduo::net::TcpConnectionPtr& conn, 
                                muduo::net::Buffer* buffer,
                                muduo::Timestamp timestamp)
{
    // 网络上接受的远程rpc调用请求的字符流
    std::string recv_buf = buffer->retrieveAllAsString();
    
    // TODO:只读取指定字节的信息
    // 从字符流中读取前四个字节的内容
    uint32_t header_size = 0;
    recv_buf.copy((char*)&header_size, 4, 0);

    std::string rpc_header_str = recv_buf.substr(4, header_size);
    mprpc::RpcHeader rpcHeader;
    std::string service_name;
    std::string method_name;
    uint32_t args_size;
    // 数据反序列化
    if (rpcHeader.ParseFromString(rpc_header_str))
    {
        service_name = rpcHeader.service_name();
        method_name = rpcHeader.methon_name();
        args_size = rpcHeader.args_size();
    }
    else 
    {
        std::cout << "rpc_header_str:" << rpc_header_str << " parse error!" << std::endl; 
        return;
    }

    // 获取rpc方法参数的字符串流数据
    std::string args_str = recv_buf.substr(4 + header_size, args_size);

    // 打印调试信息
    std::cout << "============================================" << std::endl;
    std::cout << "header_size: " << header_size << std::endl; 
    std::cout << "rpc_header_str: " << rpc_header_str << std::endl; 
    std::cout << "service_name: " << service_name << std::endl; 
    std::cout << "method_name: " << method_name << std::endl; 
    std::cout << "args_str: " << args_str << std::endl; 
    std::cout << "============================================" << std::endl;

    // 获取service对象和method对象
    auto it = m_serviceMap.find(service_name);
    if (it == m_serviceMap.end())
    {
        std::cout << service_name << " is not exist!" << std::endl;
    }

    auto mit = it->second.m_methodMap.find(method_name);
    if (mit == it->second.m_methodMap.end())
    {
        std::cout << service_name << ":" << method_name << " is not exist!" << std::endl;
        return;
    }

    // 获取service对象  
    google::protobuf::Service *service = it->second.m_service; 
    // 获取method对象  
    const google::protobuf::MethodDescriptor *method = mit->second; 

    // 生成rpc方法调用的请求request和响应response参数
    google::protobuf::Message *request = service->GetRequestPrototype(method).New();
    if (!request->ParseFromString(args_str))
    {
        std::cout << "request parse error, content:" << args_str << std::endl;
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





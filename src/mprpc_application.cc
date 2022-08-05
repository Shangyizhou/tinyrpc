#include "mprpc_application.h"
#include <iostream>
#include <unistd.h>
#include <string>

MprpcConfig MprpcApplication:: m_config;


// 参数提示信息
void ShowArgsHelp()
{
    std::cout<<"format: command -i <configfile>" << std::endl;
}

void MprpcApplication::Init(int argc, char **argv)
{
    // std::cout << "MprpcApplication::Init" << std::endl;

    if (argc < 2) 
    {
        ShowArgsHelp();
        exit(EXIT_FAILURE);
    }

    int opt = 0;
    std::string config_file;
    while ((opt = getopt(argc, argv, "i:")) != -1)
    {
        switch (opt)
        {
            case 'i':
                config_file = optarg;
                break;
            case '?':
                std::cout << "invalid args!" << std::endl;
                exit(EXIT_FAILURE);
            case ':':
                std::cout << "need <configfile>" << std::endl;
                exit(EXIT_FAILURE);
            default:
                break;
        }
    }

    std::cout << "config_file = " << config_file << std::endl;

    // 开始加载配置文件
    m_config.LoadConfigFile(config_file.c_str());

    std::cout << "===================加载配置信息==================" << std::endl;
    std::cout << "rpc_server_ip:" << m_config.Load("rpc_server_ip") << std::endl;
    std::cout << "rpc_server_port:" << m_config.Load("rpc_server_port") << std::endl;
    std::cout << "zookeeper_ip:" << m_config.Load("zookeeper_ip") << std::endl;
    std::cout << "zookeeper_port:" << m_config.Load("zookeeper_port") << std::endl;
    std::cout << "=========================================================" << std::endl;
}

MprpcApplication& MprpcApplication::GetInstance()
{
    static MprpcApplication app;
    return app;
}

MprpcConfig& MprpcApplication::GetConfig()
{
    return m_config;
}


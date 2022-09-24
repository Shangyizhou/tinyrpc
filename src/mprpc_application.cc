#include "mprpc_application.h"
#include <iostream>
#include <unistd.h>
#include <string>

MprpcConfig MprpcApplication:: m_config;


// 参数提示信息
void ShowArgsHelp()
{
    std::cout << "format: command -i <configfile>" << std::endl;
}

void MprpcApplication::Init(int argc, char **argv)
{
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

    // 开始加载配置文件
    m_config.LoadConfigFile(config_file.c_str());

    printf("=======================Loading Configure Information======================\n");
    printf("config_file:      %-10s\n", config_file.c_str());
    printf("rpc_server_ip:    %-10s\n", m_config.Load("rpc_server_ip").c_str());
    printf("rpc_server_port:  %-10s\n", m_config.Load("rpc_server_port").c_str());
    printf("zookeeper_ip:     %-10s\n", m_config.Load("zookeeper_ip").c_str());
    printf("zookeeper_port:   %-10s\n", m_config.Load("zookeeper_port").c_str());
    printf("==========================================================================\n");  
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


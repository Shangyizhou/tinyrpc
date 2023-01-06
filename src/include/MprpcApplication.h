#ifndef __MPRPC_APPLICATION_H__
#define __MPRPC_APPLICATION_H__

#include "MprpcChannel.h"
#include "MprpcController.h"
#include "ZookeeperUtil.h"

#include <memory.h>

class MprpcApplication
{
public:
    class MprpcConfig
    {
    public:
        void LoadConfigFile(const char *config_file);
        std::string Load(const std::string &key);
        // 去掉字符串前后的空格
        void Trim(std::string &src_buf);

        // 键值对储存信息
        std::unordered_map<std::string, std::string> m_configMap;        
    };

    static void Init(int argc, char **argv);
    static MprpcApplication& GetInstance();
    static MprpcConfig& GetConfig();
    
private:   
    static MprpcConfig m_config;

    MprpcApplication() = default;
    MprpcApplication(const MprpcApplication&) = delete;
    MprpcApplication(MprpcApplication&&) = delete;
    void operator=(const MprpcApplication&) = delete;
};

#endif // __MPRPC_APPLICATION_H__
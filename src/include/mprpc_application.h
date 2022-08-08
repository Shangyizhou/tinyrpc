#ifndef __MPRPC_APPLICATION_H__
#define __MPRPC_APPLICATION_H__

#include "mprpc_config.h"
#include "mprpc_channel.h"
#include "mprpc_controller.h"
#include "zookeeper_util.h"

class MprpcApplication
{
public:
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
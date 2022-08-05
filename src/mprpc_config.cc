#include "mprpc_config.h"
#include <iostream>
#include <string>

void MprpcConfig::LoadConfigFile(const char *config_file)
{
    std::cout << "start to Load Config File" << std::endl;

    // 打开文件
    FILE *pf = fopen(config_file, "r");
    if (nullptr == pf)
    {
        std::cout << config_file << " is not exist!" << std::endl;
        exit(EXIT_FAILURE);
    }

    /**
     * 1.注释
     * 2.正确的配置项 =
     * 3.去掉开头的多余的空格
     */
    while (!feof(pf))
    {
        // 读取一行，"xxxx\n"
        char buf[512] = {0};
        fgets(buf, 512, pf);

        // 去掉字符串前面多余空格
        std::string read_buf(buf);
        Trim(read_buf);

        // 判断#的注释
        if (read_buf[0] == '#' || read_buf.empty())
        {
            continue;
        }

        // 解析配置项
        int idx = read_buf.find('=');
        if (idx == -1)
        {
            continue;
        }

        std::string key;
        std::string value;
        // [0,idx)
        key = read_buf.substr(0, idx);
        // 防止这种情况出现：rpc_server_ip =127.0.0.1\n
        Trim(key);

        // rpcserverip=127.0.0.1\n
        int end_idx = read_buf.find('\n', idx);
        /**
         *            idx          endidx
         *             |             |         
         * rpcserverip = 127.0.0.1  \n
         * 截断了 \n 
         */
        value = read_buf.substr(idx+1, end_idx-idx-1);
        Trim(value);
        m_configMap.insert({key, value});
    }
}

// 查询配置项信息
std::string MprpcConfig::Load(const std::string &key)
{
    auto it = m_configMap.find(key);
    if (it == m_configMap.end())
    {
        return "";
    }
    return it->second;
}


void MprpcConfig::Trim(std::string &src_buf)
{
    // 找到第一个不是空格的字符
    int idx = src_buf.find_first_not_of(' ');
    if (idx != -1)
    {
        // 字符串前面有空格的情况
        src_buf = src_buf.substr(idx, src_buf.size()-idx);
    }

    // 去掉字符串后面多余的空格
    idx = src_buf.find_last_not_of(' ');
    if (idx != -1)
    {
        src_buf = src_buf.substr(0, idx+1);
    }
}
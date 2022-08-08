#ifndef __MPRPC_CONFIG__H
#define __MPRPC_CONFIG__H

#include <unordered_map>
#include <string>

// MprpcConfig用于读取配置文件
class MprpcConfig
{
public:
    void LoadConfigFile(const char *config_file);
    std::string Load(const std::string &key);

private:
    // 键值对储存信息
    std::unordered_map<std::string, std::string> m_configMap;
    // 去掉字符串前后的空格
    void Trim(std::string &src_buf);
};

#endif // __MPRPC_CONFIG__H
#ifndef __ZOOKEEPER_UTIL_H__
#define __ZOOKEEPER_UTIL_H__

#include <semaphore.h>
#include <zookeeper/zookeeper.h>
#include <string>

// 封装的zk客户端类
class ZkClient
{
public:
    ZkClient();
    ~ZkClient();

    // zkclient启动连接zkserver
    void Start();
    
    // 在zkserver上根据指定的path创建znode节点
    // 默认state=0是永久性节点
    void Create(const char *path, const char *data, int datalen, int state=0);
    
    // 根据参数指定的znode节点路径，或者znode节点的值
    std::string GetData(const char *path);

private:
    // zk的客户端句柄
    zhandle_t *m_zhandle;
};

// zookeeper-3.4.10
// port:2181

// ./zkServer.sh start

// sudo netstat -tanp 查看端口 
// tcp6       0      0 :::2181                 :::*                    LISTEN      27869/java          

#endif // __ZOOKEEPER_UTIL_H__
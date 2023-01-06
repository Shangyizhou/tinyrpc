# tinyrpc
## 项目说明

该项目是在 Linux 环境下基于 muduo、Protobuf 和 Zookeeper 实现的一个轻量级 RPC 框架。可以把单体架构系统的本地方法调用，重构成基于 TCP 网络通信的 RPC 远程方法调用，实现统一台机器不同进程或者不同机器之间的服务调用。

## 项目特点

- 基于 muduo 网络库实现高并发网络通信模块，作为 RPC 同步调用的基础。
- 基于 Protobuf 实现 RPC 方法调用和参数的序列化和反序列化，并根据其提供得 RPC 接口编写 RPC 服务。
- 基于 ZooKeeper 分布式协调服务中间件提供服务注册和服务发现功能。
- 设计了基于 TCP 传输的二进制协议，解决粘包问题，且能够高效传输服务名、方法名以及参数。

## 开发环境

- 操作系统：`Ubuntu 18.04.6 LTS`
- 编译器：`g++ 7.5.0`
- 编辑器：`vscode`
- 版本控制：`git`
- 项目构建：`cmake 3.10.2`

## 构建项目

运行脚本，其会自动编译项目

```shell
bash autobuild.sh 
```

## 如何使用

### 启动 zookeeper

我们需要在 zookeeper 上获取注册的服务信息，因此先必须保证启动了 zookeeper。我的 zookeeper 目录在 `/home/shang/thirdparty/zookeeper-3.4.10`。

在里面的 `bin` 目录下有客户端和服务端的启动脚本，先启动 zookeeper 服务。

```shell
root@iZwz9eojvzsrz78f673t51Z:# ./zkServer.sh start
```

此时可以在进程中查找到 zookeeper

![img](https://cdn.nlark.com/yuque/0/2022/png/26752078/1665126405491-abb39072-26ef-482d-aa98-eff0b338fe21.png)

再启动服务端脚本，启动客户端主要是为了看是否会因为我们的 RPC 框架插入新服务信息。

```shell
root@iZwz9eojvzsrz78f673t51Z:# ./zkCli.sh
```

### RPC 框架的示例

在 `bin`目录下有客户端和服务端两个文件，分别启动。

```shell
client  server  test.conf
```

### 启动服务端

```shell
./server -i test.conf
```

我们观察下打印的信息，可以看到它打印了 `test.conf` 配置文件的信息，至少说明 RPC 框架读取配置是成功的。

![img](https://cdn.nlark.com/yuque/0/2022/png/26752078/1665126529669-699ea4c3-4a91-4839-9a89-f374b839bf40.png)

重点是下面的信息，其显示在 zookeeper 上注册了服务。

![img](https://cdn.nlark.com/yuque/0/2022/png/26752078/1665126654779-03fd180a-32c2-429e-8196-242c464c348b.png)

我们去 zookeeper 上查看是否真的插入了新服务，发现确实如此。

![img](https://cdn.nlark.com/yuque/0/2022/png/26752078/1665126668882-548ef033-0dcc-4f3c-bcd9-fec1a485e3d5.png)

### 启动客户端

```shell
./client -i test.conf
```

有很多的提示信息，也是解析了配置文件，并且有许多 zookeeper 相关日志信息。这里注意最重要的几个地方，它打印显示了 `rpc Login response success:0`。响应为 0，RPC 方法调用成功！

![img](https://cdn.nlark.com/yuque/0/2022/png/26752078/1665126722724-17ce8451-fab4-46e3-9b71-c935309bb89f.png)

## 项目讲解

[（一）、集群和分布式理论讲解](./项目讲解/01.集群和分布式理论讲解.md)

[（二）、RPC 通信原理](./项目讲解/02.RPC通信原理.md)

[（三）、Protobuf 介绍](./项目讲解/03.Protobuf介绍.md)

[（四）、mprpc 框架加载配置文件](./项目讲解/04.mprpc框架加载配置文件.md)

[（五）、客户端如何发起请求](./项目讲解/05.客户端如何发起请求.md)

[（六）、如何发布本地服务为 RPC 服务](./项目讲解/06.如何发布本地服务为RPC服务.md)

[（七）、封装 RpcController 模块](./项目讲解/07.封装RpcController模块.md)

[（八）、Zookeeper 分布式协调服务](./项目讲解/08.Zookeeper分布式协调服务.md)

## 项目优化

- 引入 Nginx 负载均衡模块
- 封装 RPC 连接池，重用连接
- 增加 Router 层次，在 RPC 服务的众多服务器集群中选择某一节点
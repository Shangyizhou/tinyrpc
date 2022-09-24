#include <stdio.h>

int main()
{
    printf("=======================Loading Configure Information======================\n");
    printf("rpc_server_ip:    %-10s\n", "127.0.0.1");
    printf("rpc_server_port:  %-10s\n", "3306");
    printf("zookeeper_ip:     %-10s\n", "127.0.0.1");
    printf("zookeeper_port:   %-10s\n", "8090");
    printf("==========================================================================\n");  

    return 0;
}

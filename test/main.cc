#include <string>
#include <math.h>
#include <iostream>
using namespace std;

int main()
{
    // uint32_t header_size = 23;

    // string send_rpc_str;
    // // 这个23，存储的是字节的表现形式
    // // 一共四个字节，将每个字节标识的字符转换为字符串存到send_rpc_str中，这样就正好对应了四位
    // send_rpc_str.insert(0, std::string((char*)&header_size, 4));
    // cout << send_rpc_str[0] << " " << send_rpc_str[1];
    // printf("%s\n", send_rpc_str.c_str());
    // cout << send_rpc_str << endl;

    // uint32_t new_header_size = 0;
    // // 将send_rpc_str前四位拷贝到new_header_size
    // // 之后int类型按照四个字节读取，就会得到我们要的整数
    // send_rpc_str.copy((char*)&new_header_size, 4, 0);
    // cout << new_header_size << endl; // 23

    int header_size = 10;
    char* p = (char*)&header_size;

    return 0;
}
// g++ main.cc -g -o main
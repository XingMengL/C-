/*=============================================================== 
*   Copyright (C) . All rights reserved.")
*   文件名称： 
*   创 建 者：zhang
*   创建日期：
*   描    述：封装一个UdpSocket类，每一个实例化的对象都可以通过成员接口实现一个udp的客户端或者服务端
*    也就意味着每个实例化的对象都有一个自己的套接字描述符
================================================================*/

#include <cstdio>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

class UdpSocket {
    private:
        int _sockfd;
    public:
        UdpSocket():_sockfd(-1) {}
        bool Socket() {
            _sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            if (_sockfd < 0) {
                perror("socket error");
                return false;
            }
            return true;
        }//创建套接字
        bool Bind(const std::string &ip, const uint16_t port) {
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            socklen_t len = sizeof(addr);
            if (bind(_sockfd, (struct sockaddr *)&addr, len) < 0) {
                perror("bind error");
                return false;
            }
            return true;
        }//绑定地址信息
        //接收数据,并且根据默认参数是否为NULL，选择是否获取对端的地址信息
        bool Recv(std::string *buf, std::string *ip = NULL, uint16_t *port = NULL) {
            char tmp[4096] = {0};
            struct sockaddr_in peer_addr;
            socklen_t len = sizeof(peer_addr);
            int ret = recvfrom(_sockfd, tmp, 4096, 0, (struct sockaddr *)&peer_addr, &len);
            if (ret < 0) {
                perror("recvfrom error");
                return false;
            }
            buf->assign(tmp, ret);//从tmp中拷贝ret长度的数据到string对象中
            //ip或者port是默认参数为NULL，若用户想要获取地址和端口则传入一个空间的地址
            //否则不想获取地址信息，就不需要传入参数了，参数默认为NULL
            if (ip != NULL) {
                *ip = inet_ntoa(peer_addr.sin_addr);//将网络字节序数据转换为字符串IP地址
            }
            if (port != NULL) {
                *port = ntohs(peer_addr.sin_port);
            }
            return true;
        }
        bool Send(const std::string &data, const std::string &ip, const uint16_t port) {
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            socklen_t len = sizeof(addr);
            int ret = sendto(_sockfd, data.c_str(), data.size(), 0, (struct sockaddr*)&addr, len);
            if (ret < 0) {
                perror("sendto error");
                return false;
            }
            return true;
        }
        bool Close() {
            if (_sockfd >= 0) {
                close(_sockfd);
            }
            return true;
        }//关闭套接字
};



// 封装一个udpsocket类，每个类的实例化对象可以通过成员接口实现一个udp的客户端或者服务器也就意味着每个实例化的对象都有一个字节的套接字描述符

#include <cstdio>
#include <cstdlib>
#include <cunistd>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

class udpsock
{
  private:
    int _sockfd;
  public:
    updsock();
    bool socket();
    bool bind(std::string &ip,uint16_t  port);
    bool recv(std::string *buf, std::string *ip = NILL, uint16_t *port = NULL);
    bool sendto(std::string &data,std::string &ip,uint16_t port);
    bool close();

};

int main()
{
  udpsock sock;
  sock.socket();
  sock.bind("192.1688.1.1",900);
  while(1)
  {
  std::string buf;
  sock.recv(&buf);

  sock.sendto("192.168.1.1",900);

  }


  return 0;
}

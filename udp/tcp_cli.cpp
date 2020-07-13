#include <iostream>
#include "tcpsock.hpp"
#include <cstdlib>

int main(int argc, char *argv[])
{
  if(argc != 3)
  {
    std::cout<<"usge :./tcp_cli 198.168.1.1 9000\n";
    return -1;
  }

  std::string srv_ip = argv[1];
  uint16_t srv_port = std::stoi(argv[2]);


  TcpSock cli_sock;
  //1.创建套接字
  cli_sock.Socket();
  //2.绑定地址
  
  //3.请求连接
  cli_sock.Connect(srv_ip,srv_port);
  while(1)
  {
    std::string buf;
    std::cout<<"cli say: ";
    std::cin>>buf;
  //4.发送数据
    if(cli_sock.Send(buf) == false)
    {
        std::cout<<"send error\n";
        cli_sock.Close();
        return -1;
    }
    buf.clear();
  //5.接收数据
  if(cli_sock.Recv(&buf) == false)
  {
     std::cout<<"recv error\n"<<std::endl;
     cli_sock.Close();
     return -1;
  }
  std::cout<<"srv say : "<<buf<<std::endl;
  }
  //6.关闭套接字
  cli_sock.Close();
}


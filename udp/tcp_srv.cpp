#pragma once 
#include "tcpsock.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    std::cout<<"usged: ./tcp_src 192.168.1.1\n";
  }
  std::string ip = argv[1];
  uint16_t port = std::stoi(argv[2]);

  TcpSock srv_lis_sock;
  //1.创建套接字
  srv_lis_sock.Socket();
  //2.绑定地址
  srv_lis_sock.Bind(ip,port);
  //3.开始监听
  srv_lis_sock.Listen();
  while(1)
  {
    //获取新建连接的套接字 ----服务端可能要与多个客户端通信 
    TcpSock new_sock;
    if(srv_lis_sock.Accept(&new_sock) == false)
    {
      continue;
    }
    std::string buf;
  //5.接收数据
    if(new_sock.Recv(&buf) == false)
    {
        perror("recv error\n");
        srv_lis_sock.Close();
        continue;
    }
  
    std::cout<<"cli say:"<<buf<<std::endl;
  //6.发送数据
  buf.clear();
  std::cout<<"srv say:";
  std::cin>>buf;
  if(false == new_sock.Send(buf))
  {
    perror("send error\n");
    new_sock.Close();
  }

  }
  //7.关闭套接字
  srv_lis_sock.Close();

}

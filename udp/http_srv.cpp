#include "tcpsock.hpp"
#include <iostream>
#include <sstream>

int main(int argc, char*argv[])
{
  TcpSock lis_sock;
  lis_sock.Socket();
  
  std::string ip = "0.0.0.0";
  uint16_t port = 9000;
  
  lis_sock.Bind(ip,port);
  lis_sock.Listen();
  
  while(1)
  {
    TcpSock new_sock;
    lis_sock.Accept(&new_sock);

    std::string buf;
    new_sock.Recv(&buf);
    std::cout<<"http requset["<<buf<<"]\n"<<std::endl;

    //组织响应信息 首行 头部 空行  正文
    std::string body = "<html><body><h1>X M L</h1></body></html>";
    std::string blank = "\r\n";
    std::stringstream header;
    header << "Content-Length: "<<body.size()<<"\r\n";
    header <<"Content-Type: text/html\r\n";
    header << "Connection: close\r\n";
    std::string first = "HTTP/1.1 200 OK\r\n";

    std::string http_rsp = first+header.str()+blank + body;
    new_sock.Send(http_rsp);
    new_sock.Close();

  }
  return 0;
}

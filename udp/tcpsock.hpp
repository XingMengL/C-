#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

class TcpSock
{
  public:
    TcpSock():_sockfd(-1){}
    bool Socket()
    {
      _sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
      if(_sockfd < 0)
      {
        perror("socket error\n");
        return false;
      }
      return true;
    }
    bool Bind(std::string &ip, uint16_t port)
    {
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      socklen_t len = sizeof(addr);
      int ret = bind(_sockfd,(struct sockaddr*) &addr,len);
      if(ret < 0)
      {
          perror("bind error\n");
          return false;
      }
      return true;
    }
    bool Listen(int backlog = 10)
    {
      if(listen(_sockfd,backlog) < 0)
      {
        perror("listen error\n ");
        return false;
      }
      return true;
    }
    bool Connect(std::string &ip, uint16_t port)
    {
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      socklen_t len = sizeof(addr);

    int ret = connect(_sockfd,(struct sockaddr*) &addr, len);
     if(ret < 0)
     {
        perror("connect error\n");
        return false;
     }
        return true;
    }
    bool Accept(TcpSock* sock)
    {

      int newfd = accept(_sockfd,NULL,NULL);
      if(newfd < 0)
      {
        perror("accept error\n");
        return false;
      }
      sock->_sockfd = newfd;
      return true;
    }
    bool Recv(std::string *buf)
    {
      char temp[1024] = {0};
      int ret = recv(_sockfd, temp,1024,0);
      if(ret < 0)
      {
        perror("recv error\n");
      }
      else if(ret == 0)
      {
        std::cout<<"connect shutdown"<<std::endl;
        return false;
      }
      buf->assign(temp,ret);
      return true;
    }
    bool Send(std::string &data)
    {
      int ret = send(_sockfd,data.c_str(),data.size(),0);
        if(ret < 0)
        {
          perror("send error\n");
          return false;
        }

      return true;
   }
    bool Close()
    {
      if(_sockfd >= 0)
      {
        close(_sockfd);
        _sockfd = -1;
      }
    return true;
    }

  private:
    int _sockfd;
};


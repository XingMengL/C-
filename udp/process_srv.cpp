#include "tcpsock.hpp"
#include <iostream>
#include <signal.h>
#include <sys/wait.h>
void sigcb(int no)
{
  while(waitpid(-1,NULL,WNOHANG) > 0);
    
}
int main(int argc, char *argv[] )
{
  if(argc != 3)
  {
    std::cout<<"usage: ./process_srv 192.162.1.1\n";
    return -1;
  }
  signal(SIGCHLD,sigcb);
  std::string ip = argv[1];
  uint16_t port = std::stoi(argv[2]);

  TcpSock lis_sock;

  lis_sock.Socket();
  lis_sock.Bind(ip,port);
  lis_sock.Listen();
  while(1)
  {
    TcpSock newsock;
    lis_sock.Accept(&newsock);
   
    // 主执行流只负责获取新连接，获取新连接后就创建一个新的执行流负责与指定的客户端通信
    
    int pid = fork();
    if(pid == 0)
    {
      //子进程复制父进程，可以直接访问new_sock与客户端通信
      while(1)
      {
//子进程
    std::string buf;
   if( newsock.Recv(&buf) == false)
   {
     break;
   }
    std::cout<<"cli say:"<<buf<<std::endl;
    buf.clear();
    std::cout<<"srv say:";
    std::cin>>buf;
   if( newsock.Send(buf)==false)
   {
     break;
   }
      }
      exit(0);
    }

    //父进程并不与客户端通信，而父子进程数据独有，各有各的new_sock
    newsock.Close();// 父进程关闭newsock并不会影响子进程的通信，----数据独有
  }
  lis_sock.Close();
  return 0;
}

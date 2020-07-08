// udp通信客户端程序
// 1.创建套接字
// 2.发送数据
// 3.接收数据
// 关闭套接字

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>  // socket接口头文件
#include <netinet/in.h>

int main(int argc , char *argv[])
{
   // argc 表示程序的运行参数
   // argv 存储程序的运行参数，运行参数是运行程序的时候，以空格间隔在程序后边
   
  if(argc!=3)
  {
    printf("三个参数\n");
    return;
  }
  
  char* srv_ip = argv[1];
  uint16_t srv_port = atoi(argv[2]);

  // 创建套接字
  // socket(地址域类型，套接字类型、协议)
  int sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
  if(socked < 0)
  {
    perror("socker error");
    return -1;
  }
 // 绑定地址信息
 // bind(套接字描述符，地址信息，地址信息长度)
 /* 
  struct sockaddr_in addr; // 使用ipv4地址结构
  addr.sin_family = AF_INET;// 地址域类型
  addr.sin_port = htons(port);// 转换为网络字节序的端口信息，
  addr.sin_addr.s_addr = inet_addr(ip);

  socklen_t len = sizeof(addr);
  int ret = bind(sockfd, (struct sockaddr*)& addr, len);
  if(ret < 0)
  {
    perror("bind erro\n");
    return -1;
  }
  */
  
  struct sockaddr_in addr; // 使用ipv4地址结构
  addr.sin_family = AF_INET;// 地址域类型
  addr.sin_port = htonss(srv_port);// 转换为网络字节序的端口信息，
  addr.sin_addr.s_addr = inet_addr(srv_ip);
  socklen_t len = sizeof(addr);
  while(1)
  {
    // 发送数据
    // sendto（套接字描述符，要发送的数据，发送数据的长度、标志位（置0），服务端地址信息，服务端地址信息长度）
    char temp[1024]= {0};
    printf("client say:");
    sacnf("%s",temp);
    ret = sendto(sockfd,temp.strlen(tmp),0,(struct sockaddr*)&addr,len);
    if(ret < 0)
    {
      perror("sendto error\n");
      close(sockfd);
      return -1
    }

    char buf[1024] = {0};
    // 对于客户端来说，知道了服务端的地址，所以没必要接收服务端的地址信息
    ret = recvfrom(sockfd,buf,1023,0,NULL.0);
    if(ret < 0)
    {
      perror("recvfrom error\n");
      close(sockfd);
      return -1
    }
  }
  close(sockfd);
  return 0

}


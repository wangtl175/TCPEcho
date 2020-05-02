/* TCPServer.cpp - main */
/*
接收客户端发来的信息
发出时间
发出信息
*/
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <time.h>
#include <conio.h>

#define	WSVERS	MAKEWORD(2, 0)
#define BUFLEN 2000
#pragma comment(lib,"ws2_32.lib")  //使用winsock 2.2 library
void main(int argc, char *argv[])
{
	struct	sockaddr_in fsin;	    /* the from address of a client	  */
	SOCKET	msock, ssock;		    /* master & slave sockets	      */
	WSADATA wsadata;
	char *service = "50500";
	struct  sockaddr_in sin;	    /* an Internet endpoint address		*/
	int	    alen;			        /* from-address length		        */
	char *pts;			        /* pointer to time string	        */
	time_t	now;			        /* current time			            */
	char buf[BUFLEN + 2];
	int rc, sc;

	WSAStartup(WSVERS, &wsadata);						// 加载winsock library。WSVERS指明请求使用的版本。wsadata返回系统实际支持的最高版本
	msock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	// 创建套接字，参数：因特网协议簇(family)，流套接字，TCP协议
														// 返回：要监听套接字的描述符或INVALID_SOCKET

	memset(&sin, 0, sizeof(sin));						// 从&sin开始的长度为sizeof(sin)的内存清0
	sin.sin_family = AF_INET;							// 因特网地址簇(INET-Internet)
	sin.sin_addr.s_addr = INADDR_ANY;					// 监听所有(接口的)IP地址。
	sin.sin_port = htons((u_short)atoi(service));		// 监听的端口号。atoi--把ascii转化为int，htons--主机序到网络序(host to network，s-short 16位)
	if (bind(msock, (struct sockaddr *) & sin, sizeof(sin)) == 0)  // 绑定监听的IP地址和端口号，如果绑定成功，则服务器启动完成
	{
		printf("服务器已启动!\n\n");
		listen(msock, 5);                                   // 建立长度为5的连接请求队列，并把到来的连接请求加入队列等待处理。

		while (!_kbhit()) { 		                                   // 检测是否有按键,如果没有则进入循环体执行
			alen = sizeof(struct sockaddr);                         // 取到地址结构的长度
			ssock = accept(msock, (struct sockaddr *) & fsin, &alen); // 如果在连接请求队列中有连接请求，则接受连接请求并建立连接，返回该连接的套接字，否则，本语句被阻塞直到队列非空。fsin包含客户端IP地址和端口号
			//ssock的端口和msock相同
			(void)time(&now);                                      // 取得系统时间
			pts = ctime(&now);                                      // 把时间转换为字符串
			rc = recv(ssock, buf, BUFLEN, 0);//收到信息
			if (rc == SOCKET_ERROR)
			{
				printf("Error: %d.\n", GetLastError());
			}
			else if (rc == 0)
			{
				printf("Client closed!");
			}
			else if (rc > 0)
			{
				buf[rc] = '\n';
				rc++;
				buf[rc] = '\0';
				printf("收到消息：%s", buf);
				printf("收到时间：%s", pts);
				char content[BUFLEN + 8] = "内容：";//客户端发送的信息
				char rtime[50] = "时间：";//时间
				char ip[50] = "客户端IP地址：";//ip地址
				char port[50] = "客户端端口号：";//端口
				sprintf(ip + 14, "%d.%d.%d.%d\n", fsin.sin_addr.S_un.S_un_b.s_b1,//s_un_b获取ip地址
					fsin.sin_addr.S_un.S_un_b.s_b2,
					fsin.sin_addr.S_un.S_un_b.s_b3,
					fsin.sin_addr.S_un.S_un_b.s_b4);
				sprintf(port + 14, "%d\n", fsin.sin_port);
				strcpy(content + 6, buf);
				strcpy(rtime + 6, pts);
				printf("%s", ip);
				printf("%s\n", port);
				sc = send(ssock, content, strlen(content), 0);//发回客户端
				sc = send(ssock, rtime, strlen(rtime), 0);
				sc = send(ssock, ip, strlen(ip), 0);
				sc = send(ssock, port, strlen(port), 0);
			}
			(void)closesocket(ssock);                              // 关闭连接套接字
		}
	}
	else
	{
		printf("启动失败!\nError: %d\n", GetLastError());
	}
	(void)closesocket(msock);                                 // 关闭监听套接字
	WSACleanup();                                             // 卸载winsock library
	printf("按回车键继续...");
	getchar();										// 等待任意按键
}
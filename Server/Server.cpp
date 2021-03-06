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
#pragma comment(lib,"ws2_32.lib")  //使用winsock 2.2 library，用了这个语句就不用在项目属性里设置

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
	if (bind(msock, (struct sockaddr *) & sin, sizeof(sin))==0)  // 绑定监听的IP地址和端口号，如果绑定成功，则服务器启动完成
	{
		printf("服务器已启动!\n\n");
		listen(msock, 5);                                   // 建立长度为5的连接请求队列，并把到来的连接请求加入队列等待处理。

		while (!_kbhit()) { 		                                   // 检测是否有按键,如果没有则进入循环体执行
			alen = sizeof(struct sockaddr);                         // 取到地址结构的长度
			ssock = accept(msock, (struct sockaddr *) & fsin, &alen); // 如果在连接请求队列中有连接请求，则接受连接请求并建立连接，返回该连接的套接字，否则，本语句被阻塞直到队列非空。fsin包含客户端IP地址和端口号
			//ssock的端口和msock相同
			/*
			struct sockaddr_in connectAddr, listenAddr;
			int connectLen = sizeof(connectAddr), listenLen = sizeof(listenAddr);
			getsockname(ssock, (struct sockaddr *) & connectAddr, &connectLen);//50500
			getsockname(msock, (struct sockaddr *) & listenAddr, &listenLen);//50500
			printf("connect port: %d\nlisten port: %d\n", ntohs(connectAddr.sin_port), ntohs(listenAddr.sin_port));*/

			/*struct sockaddr_in peerAddr;
			int peerLen = sizeof(peerAddr);
			getpeername(ssock, (struct sockaddr *) & peerAddr, &peerLen);
			printf("getpeeername: %d\n", ntohs(peerAddr.sin_port));
			printf("sockaddr: %d", ntohs(fsin.sin_port));//两个是一样的*/

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
				sc = send(ssock, pts, strlen(pts), 0);
				sc = send(ssock, buf, strlen(buf), 0);
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

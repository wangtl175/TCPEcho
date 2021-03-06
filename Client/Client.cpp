/* TCPClient.cpp */

#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>

#define	BUFLEN		2000                  // 缓冲区大小
#define WSVERS		MAKEWORD(2, 0)        // 指明版本2.0 
#pragma comment(lib,"ws2_32.lib")         // 使用winsock 2.0 Llibrary
void main(int argc, char *argv[])
{
    char *host = "127.0.0.1";	    /* server IP to connect         */
    char *service = "50500";  	    /* server port to connect       */
    struct sockaddr_in sin;	            /* an Internet endpoint address	*/
    char	buf[BUFLEN + 1];   		    /* buffer for one line of text	*/
    SOCKET	sock;		  	            /* socket descriptor	    	*/
    int	cc;			                    /* recv character count		    */

    WSADATA wsadata;
    WSAStartup(WSVERS, &wsadata);						  //加载winsock library。WSVERS为请求的版本，wsadata返回系统实际支持的最高版本

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	  //创建套接字，参数：因特网协议簇(family)，流套接字，TCP协议
                                                          //返回：要监听套接字的描述符或INVALID_SOCKET

    memset(&sin, 0, sizeof(sin));						  // 从&sin开始的长度为sizeof(sin)的内存清0
    sin.sin_family = AF_INET;							  // 因特网地址簇
    sin.sin_addr.s_addr = inet_addr(host);                // 设置服务器IP地址(32位)
    sin.sin_port = htons((u_short)atoi(service));         // 设置服务器端口号  
    int ret = connect(sock, (struct sockaddr *) & sin, sizeof(sin));  // 连接到服务器，第二个参数指向存放服务器地址的结构，第三个参数为该结构的大小，返回值为0时表示无错误发生，返回SOCKET_ERROR表示出错，应用程序可通过WSAGetLastError()获取相应错误代码。

    if (ret == SOCKET_ERROR)
    {
        printf("Error:%d.\n", GetLastError());
    }
    else
    {
        printf("输入要发送的消息：");
        char str[10];                      /*text to send*/
        scanf("%s", str);
        int sc = send(sock, str, strlen(str), 0);
        printf("\n收到消息：\n");
        //int k = 0;
        //Sleep(500);
        while (true)//用循环接收信息，当出错或连接关闭时退出循环
        {
            cc = recv(sock, buf, BUFLEN, 0);
            if (cc == SOCKET_ERROR)
            {
                printf("Error: %d.\n", GetLastError());
                break;
            }
            else if (cc == 0)
            {
                break;//服务器关闭，传输完成
            }
            else if (cc > 0)
            {
                k++;
                buf[cc] = '\0';
                printf("%s", buf);
            }
        }
        //printf("接收了：%d次\n", k);//虽然Server2发了4次，但客户端不一定接收四次，因为TCP是字节流方式传输
    }
    closesocket(sock);                             // 关闭监听套接字
    WSACleanup();                                  // 卸载winsock library

    printf("按回车键继续...");
    getchar();										// 等待任意按键
    getchar();
}
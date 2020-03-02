/* TCPServer.cpp - main */
/*
���տͻ��˷�������Ϣ
����ʱ��
������Ϣ
*/
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <time.h>
#include "conio.h"

#define	WSVERS	MAKEWORD(2, 0)
#define BUFLEN 2000
#pragma comment(lib,"ws2_32.lib")  //ʹ��winsock 2.2 library
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

	WSAStartup(WSVERS, &wsadata);						// ����winsock library��WSVERSָ������ʹ�õİ汾��wsadata����ϵͳʵ��֧�ֵ���߰汾
	msock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	// �����׽��֣�������������Э���(family)�����׽��֣�TCPЭ��
														// ���أ�Ҫ�����׽��ֵ���������INVALID_SOCKET

	memset(&sin, 0, sizeof(sin));						// ��&sin��ʼ�ĳ���Ϊsizeof(sin)���ڴ���0
	sin.sin_family = AF_INET;							// ��������ַ��(INET-Internet)
	sin.sin_addr.s_addr = INADDR_ANY;					// ��������(�ӿڵ�)IP��ַ��
	sin.sin_port = htons((u_short)atoi(service));		// �����Ķ˿ںš�atoi--��asciiת��Ϊint��htons--������������(host to network��s-short 16λ)
	if (bind(msock, (struct sockaddr *) & sin, sizeof(sin))==0)  // �󶨼�����IP��ַ�Ͷ˿ںţ�����󶨳ɹ�����������������
	{
		printf("������������!\n\n");
		listen(msock, 5);                                   // ��������Ϊ5������������У����ѵ������������������еȴ�������

		while (!_kbhit()) { 		                                   // ����Ƿ��а���,���û�������ѭ����ִ��
			alen = sizeof(struct sockaddr);                         // ȡ����ַ�ṹ�ĳ���
			ssock = accept(msock, (struct sockaddr *) & fsin, &alen); // ����������������������������������������󲢽������ӣ����ظ����ӵ��׽��֣����򣬱���䱻����ֱ�����зǿա�fsin�����ͻ���IP��ַ�Ͷ˿ں�
			//ssock�Ķ˿ں�msock��ͬ
			(void)time(&now);                                      // ȡ��ϵͳʱ��
			pts = ctime(&now);                                      // ��ʱ��ת��Ϊ�ַ���
			rc = recv(ssock, buf, BUFLEN, 0);//�յ���Ϣ
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
				printf("�յ���Ϣ��%s", buf);
				printf("�յ�ʱ�䣺%s", pts);
				sc = send(ssock, pts, strlen(pts), 0);
				sc = send(ssock, buf, strlen(buf), 0);
			}
			(void)closesocket(ssock);                              // �ر������׽���
		}
	}
	else
	{
		printf("����ʧ��!\nError: %d\n", GetLastError());
	}
	(void)closesocket(msock);                                 // �رռ����׽���
	WSACleanup();                                             // ж��winsock library
	printf("���س�������...");
	getchar();										// �ȴ����ⰴ��
}
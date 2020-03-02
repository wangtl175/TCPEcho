/* TCPClient.cpp */

#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>

#define	BUFLEN		2000                  // ��������С
#define WSVERS		MAKEWORD(2, 0)        // ָ���汾2.0 
#pragma comment(lib,"ws2_32.lib")         // ʹ��winsock 2.0 Llibrary
void main(int argc, char *argv[])
{
    char *host = "127.0.0.1";	    /* server IP to connect         */
    char *service = "50500";  	    /* server port to connect       */
    struct sockaddr_in sin;	            /* an Internet endpoint address	*/
    char	buf[BUFLEN + 1];   		    /* buffer for one line of text	*/
    SOCKET	sock;		  	            /* socket descriptor	    	*/
    int	cc;			                    /* recv character count		    */

    WSADATA wsadata;
    WSAStartup(WSVERS, &wsadata);						  //����winsock library��WSVERSΪ����İ汾��wsadata����ϵͳʵ��֧�ֵ���߰汾

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	  //�����׽��֣�������������Э���(family)�����׽��֣�TCPЭ��
                                                          //���أ�Ҫ�����׽��ֵ���������INVALID_SOCKET

    memset(&sin, 0, sizeof(sin));						  // ��&sin��ʼ�ĳ���Ϊsizeof(sin)���ڴ���0
    sin.sin_family = AF_INET;							  // ��������ַ��
    sin.sin_addr.s_addr = inet_addr(host);                // ���÷�����IP��ַ(32λ)
    sin.sin_port = htons((u_short)atoi(service));         // ���÷������˿ں�  
    int ret = connect(sock, (struct sockaddr *) & sin, sizeof(sin));  // ���ӵ����������ڶ�������ָ���ŷ�������ַ�Ľṹ������������Ϊ�ýṹ�Ĵ�С������ֵΪ0ʱ��ʾ�޴�����������SOCKET_ERROR��ʾ����Ӧ�ó����ͨ��WSAGetLastError()��ȡ��Ӧ������롣

    if (ret == SOCKET_ERROR)
    {
        printf("Error:%d.\n", GetLastError());
    }
    else
    {
        printf("����Ҫ���͵���Ϣ��");
        char str[10];                      /*text to send*/
        scanf("%s", str);
        int sc = send(sock, str, strlen(str), 0);
        printf("\n�յ���Ϣ��\n");
        while (true)//��ѭ��������Ϣ������������ӹر�ʱ�˳�ѭ��
        {
            cc = recv(sock, buf, BUFLEN, 0);
            if (cc == SOCKET_ERROR)
            {
                printf("Error: %d.\n", GetLastError());
                break;
            }
            else if (cc == 0)
            {
                break;//�������رգ��������
            }
            else if (cc > 0)
            {
                buf[cc] = '\0';
                printf("%s", buf);
            }
        }
    }
    closesocket(sock);                             // �رռ����׽���
    WSACleanup();                                  // ж��winsock library

    printf("���س�������...");
    getchar();										// �ȴ����ⰴ��
    getchar();
}
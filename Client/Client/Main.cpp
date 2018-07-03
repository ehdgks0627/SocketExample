#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>
#pragma comment (lib, "Ws2_32.lib")
#pragma warning(disable: 4996)

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8282 

void error_exit(const char *message);
typedef struct _packet
{
	char command[16];
	int x;
	int y;
}Packet;

int main(int argc, char *argv[])
{
	SOCKET clientsock;
	WSADATA wsa;
	struct sockaddr_in sockinfo;
	int choice, iRet;
	Packet requestPacket, responsePacket;

	
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		error_exit("초기화 실패\n");
	clientsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if (clientsock == INVALID_SOCKET)
		error_exit("소켓 생성 실패\n")
		;
	memset(&sockinfo, 0, sizeof(sockinfo));
	sockinfo.sin_family = AF_INET;
	sockinfo.sin_port = htons(SERVER_PORT);
	sockinfo.sin_addr.s_addr = inet_addr(SERVER_ADDR);
	
	if (connect(clientsock, (SOCKADDR*)&sockinfo, sizeof(sockinfo)) == SOCKET_ERROR)
		error_exit(" 서버 접속 실패 ");

	choice = iRet = 0;
	while (choice != 3 && iRet != -1)
	{
		memset(&requestPacket, 0x00, sizeof(Packet));
		memset(&responsePacket, 0x00, sizeof(Packet));
		printf("1. 위치 받아오기(get_position)\n");
		printf("2. 위치 설정하기(set_position)\n");
		printf("3. 종료\n");
		scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			strcpy(requestPacket.command, "get_position");
			send(clientsock, (char*)&requestPacket, sizeof(Packet), 0);
			iRet = recv(clientsock, (char*)&responsePacket, sizeof(Packet), 0);
			if (iRet == -1)
			{
				printf("서버와 연결 끊김\n");
				break;
			}
			if (!strcmp("ok", responsePacket.command))
			{
				printf("서버에서 받아온 플레이어의 좌표 : %d %d\n", responsePacket.x, responsePacket.y);
			}
			else
			{
				printf("에러\n");
			}
			break;
		case 2:
			strcpy(requestPacket.command, "set_position");
			printf("x좌표 >>> ");
			scanf("%d", &requestPacket.x);
			printf("y좌표 >>> ");
			scanf("%d", &requestPacket.y);
			send(clientsock, (char*)&requestPacket, sizeof(Packet), 0);
			iRet = recv(clientsock, (char*)&responsePacket, sizeof(Packet), 0);
			if (iRet == -1)
			{
				printf("서버와 연결 끊김\n");
				break;
			}
			if (!strcmp("ok", responsePacket.command))
			{
				printf("설정 완료\n");
			}
			else
			{
				printf("설정 실패\n");
			}
			break;
		default:
			break;
		}
	}
	
	closesocket(clientsock);


	WSACleanup();
}
void error_exit(const char *message)
{
	printf("%s\n", message);
	exit(1);
}
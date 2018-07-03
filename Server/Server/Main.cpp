#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#pragma comment (lib, "Ws2_32.lib")
#pragma warning(disable: 4996)

#define LISTEN_PORT 8282

void error_exit(const char *message);

typedef struct _packet
{
	char command[16];
	int x;
	int y;
}Packet;

int main(int argc, char *argv[])
{

	SOCKET sock, clientsock;
	WSADATA wsa;
	struct sockaddr_in sockinfo, clientinfo;
	int clientsize;
	Packet requestPacket, responsePacket;

	int x, y; // ĳ������ ��ġ�� ���� ����

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		error_exit("�ʱ�ȭ ����\n");

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == INVALID_SOCKET)
		error_exit("���� ���� ����\n");

	memset(&sockinfo, 0, sizeof(sockinfo));

	sockinfo.sin_family = AF_INET;
	sockinfo.sin_port = htons(LISTEN_PORT);
	sockinfo.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sock, (SOCKADDR*)&sockinfo, sizeof(sockinfo)) == SOCKET_ERROR)
		error_exit(" bind ���� "); // bind�� �����ϴ� ��� �̹� ��Ʈ�� ������̰ų� ���α׷��� �ߺ�����Ǿ��� ���ɼ��� ����

	if (listen(sock, 5) == SOCKET_ERROR)
		error_exit(" ��⿭ ���� ");

	clientsize = sizeof(clientinfo);

	printf("Ŭ���̾�Ʈ�κ��� ������ ��ٸ��� �ֽ��ϴ�...\n");

	clientsock = accept(sock, (SOCKADDR*)&clientinfo, &clientsize);

	if (clientsock == INVALID_SOCKET)
		error_exit(" Ŭ���̾�Ʈ ���� ���� ���� ");
	else
		printf("Ŭ���̾�Ʈ ����\n");
	x = y = 0;
	while (1)
	{
		memset(&requestPacket, 0x00, sizeof(Packet));
		memset(&responsePacket, 0x00, sizeof(Packet));
		if (recv(clientsock, (char*)&requestPacket, sizeof(Packet), 0) == -1)
		{
			break;
		}
		if (!strcmp("set_position", requestPacket.command))
		{
			strcpy(responsePacket.command, "ok");
			x = requestPacket.x;
			y = requestPacket.y;
		}
		else if (!strcmp("get_position", requestPacket.command))
		{
			strcpy(responsePacket.command, "ok");
			responsePacket.x = x;
			responsePacket.y = y;
		}
		else
		{
			strcpy(responsePacket.command, "error");
		}
		send(clientsock, (char*)&responsePacket, sizeof(Packet), 0);
	}

	closesocket(sock);
	closesocket(clientsock);

	WSACleanup();
}

void error_exit(const char *message)
{
	printf("%s\n", message);
	exit(1);
}
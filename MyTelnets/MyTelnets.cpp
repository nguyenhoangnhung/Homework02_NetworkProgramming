#include "pch.h"
#include <iostream>
#include <WinSock2.h>
using namespace std;
DWORD WINAPI ClientThread(LPVOID);
void Disconnect(SOCKET);
void openFile();

CRITICAL_SECTION cs;
SOCKET listClients[64];
char username[64][64];
char password[64][64];
int account = 0;

int no_client = 0;

int main(int argc, char * argv[]) {
	openFile();
	WSADATA wsa;
	int ret = WSAStartup(0x0202, &wsa);
	if (ret == 0) {
		cout << "Initialize WSA" << endl;
	}
	else {
		cout << "Error while start up ";
		return -4;
	}
	if (!InitializeCriticalSectionAndSpinCount(&cs, 0x00000400)) {
		cout << "Error while intialize CRITICAL_SECTION";
		return -3;
	}
	int port = atoi(argv[1]);
	if (port <= 0) {
		cout << "Wrong port!!";
		return -1;
	}
	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addrin;
	addrin.sin_family = AF_INET;
	addrin.sin_addr.s_addr = htonl(INADDR_ANY);
	addrin.sin_port = htons(port);

	bind(server, (SOCKADDR*)&addrin, sizeof(addrin));
	listen(server, 5);
	while (true) {
		SOCKET client = accept(server, NULL, NULL);
		CreateThread(0, 0, ClientThread, &client, 0, 0);
	}
	closesocket(server);
	for (int i = 0; i < no_client; i++)
	{
		closesocket(listClients[i]);
	}
	WSACleanup();
	DeleteCriticalSection(&cs);
	return 0;
}

DWORD WINAPI ClientThread(LPVOID s) {
	SOCKET client = *(SOCKET*)s;
	char buf[256];
	int ret;
	char u[64], p[64];
	while (true) {
		bool login = false;
		strcpy(buf, "Moi ban nhap username va password: ");
		send(client, buf, strlen(buf), 0);
		ret = recv(client, buf, sizeof(buf), 0);
		if (ret <= 0) {
			Disconnect(client);
			return 0;
		}
		sscanf(buf, "%s%s", u, p);
		for (int i = 0; i < account; i++)
		{
			if (strcmp(u, username[i]) == 0) {
				if (strcmp(p, password[i]) == 0) {
					login = true;
					cout << u << " login successfully!" << endl;
					break;
				}
			}
		}
		if (login) break;
		else {
			strcpy(buf, "Sai tai khoan mat khau!!");
			send(client, buf, strlen(buf), 0);
			send(client, 0, 1, 0);
		}
	}

	const char output[] = ">\"C:\\Users\\Thu Hong\\source\\repos\\HW\\MyTelnets\\out.txt\"";
	while (true) {
		char buf[256];
		ret = recv(client, buf, sizeof(buf), 0);
		if (ret <= 0) {
			Disconnect(client);
			return 0;
		}
		buf[ret - 1] = ' ';
		for (int i = ret; i < ret + strlen(output); i++)
		{
			buf[i] = output[i - ret];
		}
		buf[ret + strlen(output)] = 0;
		cout << "Command: " << buf << endl;
		EnterCriticalSection(&cs);
		system(buf);
		FILE *f;
		f = fopen("C:\\Users\\Thu Hong\\source\\repos\\HW\\MyTelnets\\out.txt", "r");
		do {
			ret = fread(buf, 1, 1, f);
			buf[ret] = 0;
			cout << buf;
			send(client, buf, strlen(buf), 0);
		} while (ret > 0);
		fclose(f);
		LeaveCriticalSection(&cs);
	}
	return 0;
}

void Disconnect(SOCKET s) {
	cout << s << " disconnected" << endl;
	int i = 0;
	for (; i < no_client; i++) {
		if (listClients[i] == s)
			break;
	}
	if (i < no_client - 1) {
		listClients[i] = listClients[no_client - 1];
	}
	no_client--;
}

void openFile() {
	FILE* f;
	f = fopen("C:\\Users\\Thu Hong\\source\\repos\\HW\\MyTelnets\\data.txt", "r");
	int ret = 0;
	while (true) {
		ret = fscanf(f, "%s%s", username[account], password[account]);
		if (ret < 0) break;
		account++;
	}
	for (int i = 0; i < account; i++)
	{
		cout << username[i] << ":" << password[i] << endl;
	}
	fclose(f);
}
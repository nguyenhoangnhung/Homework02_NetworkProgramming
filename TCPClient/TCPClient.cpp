#include "pch.h"

int main(int argc, char * argv[])
{
	char * address = argv[1];
	int port = atoi(argv[2]);
	WSADATA wsa;
	int ret = WSAStartup(0x0202, &wsa);
	if (ret == 0) {
		cout << "Initialized WSA" << endl;
	}
	else {
		cout << "Error: " << ret;
	}

	SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN addr;

	addrinfo * addr_info;
	ret = getaddrinfo(address, "http", NULL, &addr_info);
	if (ret == 0) {
		memcpy(&addr, addr_info->ai_addr, addr_info->ai_addrlen);
	}
	else {
		cout << "Can not resolve " << address << endl;
		system("pause");
		return 1;
	}

	cout << "Connecting to " << inet_ntoa(addr.sin_addr) << ":" << port << endl;
	do {
		ret = connect(client, (SOCKADDR*)&addr, sizeof(addr));
		if (ret == 0) break;
		cout << "Can not connect to " << address << ":" << port << endl;
		cout << "Enter to reconnect" << endl;
		system("pause");
	} while (true);

	cout << "Connected to " << inet_ntoa(addr.sin_addr) << ":" << port << endl;
	char buf[256];
	while (1) {
		printf("Send to %s:%d (`exit` to quit): ", address, port);
		cin >> buf;
		if (strncmp("exit", buf, 4) == 0) break;
		else {
			ret = send(client, buf, strlen(buf), 0);
			if (ret <= 0) {
				cout << "Connection loss ... Exiting ... ";
				system("pause");
				return 1;
			}
		}
	}
	closesocket(client);
	WSACleanup();
	system("pause");
	return 0;
}

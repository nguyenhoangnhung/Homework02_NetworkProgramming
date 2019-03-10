#include "pch.h"
struct Info {
	char computerName[1024];
	char bitmask[26];
	long long totalSize[26];
	long long freeSpace[26];
};
int main(int argc, char * argv[])
{
	if (argc < 2) {
		cout << "Missing argument!";
		system("pause");
		return 1;
	}
	WSADATA wsa;
	WSAStartup(0x0202, &wsa);
	int port = atoi(argv[1]);
	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);

	bind(server, (SOCKADDR*)&addr, sizeof(addr));
	listen(server, 5);

	cout << "Listen at port : " << port << "..." << endl;
	SOCKET client = accept(server, NULL, NULL);
	cout << "Connected!" << endl;

	Info info;
	int ret = recv(client, (char*)&info, sizeof(info), 0);
	cout << "Info received from client: " << endl;
	cout << "Computer name: " << info.computerName << endl;
	cout << "Disk information: " << endl;
	for (int i = 0; i < 26; i++)
	{
		if (info.bitmask[i] == '0') continue;
		char drive = 'A' + i;
		cout << drive << ":\\" << endl;
		cout << "\tFreespace " << info.freeSpace[i] << " Bytes" << endl;
		cout << "\tTotalsize " << info.totalSize[i] << " Bytes" << endl;
	}
	closesocket(client);
	closesocket(server);
	WSACleanup();
	system("pause");
}
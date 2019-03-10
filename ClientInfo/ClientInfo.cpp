#include "pch.h"
struct Info {
	char computerName[1024];
	char bitmask[26];
	long long totalSize[26];
	long long freeSpace[26];
};
int main(int argc, char * argv[])
{
	if (argc < 3) {
		cout << "Missing argument!!";
		system("pause");
		return 1;
	}

	char * ip = argv[1];
	int port = atoi(argv[2]);

	cout << "Address: " << ip << endl;
	cout << "Port: " << port << endl;

	WSADATA wsa;
	WSAStartup(0x0202, &wsa); // MAKEWORD(2,2);
	SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr;
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	int ret = 1;
	while (ret != 0) {
		ret = connect(client, (SOCKADDR*)&addr, sizeof(addr));
		if (ret == 0) {
			cout << "Connected to server!" << endl;
			break;
		}
		else {
			cout << "Failed to connect ... Enter to reconnect ... " << endl;
			system("pause");
		}
	}

	Info info;
	//computer name
	TCHAR bufName[1024];
	DWORD bufLen = 1024;
	GetComputerName(bufName, &bufLen);
	wcstombs(info.computerName, bufName, wcslen(bufName) + 1);
	// disk information 
	string buff = bitset<26>((int)GetLogicalDrives()).to_string(); 
	reverse(buff.begin(), buff.end());
	const char * t = buff.c_str();
	strncpy(info.bitmask, t, 26);
	info.bitmask[26] = 0;
	long long total, free;
	for (int i = 0; i < 26; i++)
	{
		if (info.bitmask[i] == '0') continue;
		total = 0;
		free = 0;
		char drive = 'A' + i;
		char path[4] = { drive,':','\\' };
		wchar_t temp[4];
		mbstowcs(temp, path, 5);//Plus null
		LPWSTR ptr = temp;
		GetDiskFreeSpaceEx(ptr, NULL, (PULARGE_INTEGER)&total, (PULARGE_INTEGER)&free);
		info.freeSpace[i] = free;
		info.totalSize[i] = total;
	}


	cout << "Info send to server: " << endl;
	cout << "Computer name: " << info.computerName << endl;

	//send info
	send(client, (char*)&info, sizeof(info), 0);
	cout << endl << "Sent to " << ip << ":" << port << endl;
	closesocket(client);
	WSACleanup();
	system("pause");
}
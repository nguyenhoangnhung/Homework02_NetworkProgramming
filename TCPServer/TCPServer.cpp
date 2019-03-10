#include "pch.h"

int main(int argc, char * argv[])
{
	char * port = argv[1];
	char * send_file = argv[2]; 
	char * recv_file = argv[3];

	WSADATA wsa; // wsaDATA
	int ret = WSAStartup(0x0202, &wsa); // 0 = success, error code 
	
	if (ret == 0) { // 1 = true, con lai = false 
		cout << "Initialized WSA" << endl;
	}
	else {
		cout << "Error: " << ret;
	}

	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 

	SOCKADDR_IN addr; // socket address information 

	addr.sin_family = AF_INET;//IPv4
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(atoi(port));// addr.sin_port = htons(80);

	bind(server, (SOCKADDR*)&addr, sizeof(addr));

	listen(server, 5);
	cout << "Waiting connection in port " << port << endl;
	SOCKET client = accept(server, NULL, NULL);
	cout << "Connected!" << endl;

	// send content in file to client
	char buf1[256];
	FILE *f = fopen(send_file, "r");
	if (f == NULL) {
		cout << "Error reading file ..." << endl;
		system("pause");
		return 1;
	}
	cout << "Sending to client... " << endl;
	while (1) {
		ret = fread(buf1, 1, sizeof(buf1), f);
		if (ret > 0) send(client, buf1, ret, 0); // buf1 = "thien" ret=5 flag 
		else break;
	}
	cout << "Sent!" << endl;

	//save received content
	char buf2[1024];
	FILE * g = fopen(recv_file, "w");
	ret = recv(client, buf2, sizeof(buf2), 0);
	if (ret < 1024) buf2[ret] = 0; 
	cout << "Received: " << buf2 << endl;
	fwrite(buf2, 1, ret, g);
	fclose(f);
	fclose(g);

	closesocket(client);
	closesocket(server);
	WSACleanup();
	system("pause");
	return 0;
}
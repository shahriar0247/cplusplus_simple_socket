#include <iostream>
#include <WS2tcpip.h>
#include <string>

#pragma comment (lib, "ws2_32.lib")

using namespace std;


int main()
{

	string ip = "127.0.0.1";

	// initialze winsock

	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);

	if (wsOk != 0) {
		cerr << "Cant initilize winsock, quitting" << endl;
		return 1;
	}

	// create a socket

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == INVALID_SOCKET) {
		cerr << "Cant create socket, quitting" << endl;
		WSACleanup();
		return 1;
	}

	// bind socket

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(4444);
	inet_pton(AF_INET, ip.c_str(), &hint.sin_addr);

	// connect to server

	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		cerr << "Cant connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return 1;
	}
	

	

	// while loop: accpet and echo message back to client

	char buf[4096];
	char buf2[1024] = "nice";;
	while (true) {
		ZeroMemory(buf, 4096);

		//wait for client to send data
		send(sock, buf2, sizeof(buf2), 0);

		int bytesRecv = recv(sock, buf, 4096, 0);
		cout << buf << endl;

		if (bytesRecv == SOCKET_ERROR) {
			cerr << "Error in recv(), Quitting" << endl;
			break;
		}

		if (bytesRecv == 0) {
			cout << "Client disconnected" << endl;
			break;
		}



		// echo message back to client
	}

	// close socket

	closesocket(sock);

	// cleanup winsock

	WSACleanup();
}


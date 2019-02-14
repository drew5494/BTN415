#include <windows.networking.sockets.h>
#include <iostream>
#include <fstream>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

struct packet {
	unsigned char header;
	unsigned char srcdest;
	unsigned char length;
	unsigned char data[101];
};



void print(std::ostream& os, const char* msg)
{
	os << msg << std::endl;
	std::cout << msg << std::endl;
}

int main()
{
	std::ofstream ofs("Server_Output.txt");
	if (!ofs.is_open())
		std::cout << "ERROR: Failed to open output file" << std::endl;

	//starts Winsock DLLs		
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		print(ofs, "ERROR:  Failed to start WSA");
		return 0;
	}

	//create server socket
	SOCKET ServerSocket;
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ServerSocket == INVALID_SOCKET) {
		WSACleanup();
		print(ofs, "ERROR:  Failed to create ServerSocket");
		return 0;
	}

	//binds socket to address
	sockaddr_in SvrAddr;
	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_addr.s_addr = INADDR_ANY;
	SvrAddr.sin_port = htons(27000);
	if (bind(ServerSocket, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr)) == SOCKET_ERROR)
	{
		closesocket(ServerSocket);
		WSACleanup();
		print(ofs, "ERROR:  Failed to bind ServerSocket");
		return 0;
	}

	//listen on a socket
	if (listen(ServerSocket, 1) == SOCKET_ERROR) {
		closesocket(ServerSocket);
		WSACleanup();
		print(ofs, "ERROR:  listen failed to configure ServerSocket");
		return 0;
	}


	print(ofs, "Waiting for client connection");

	//accepts a connection from a client
	SOCKET ConnectionSocket;
	ConnectionSocket = SOCKET_ERROR;
	if ((ConnectionSocket = accept(ServerSocket, NULL, NULL)) == SOCKET_ERROR) {
		closesocket(ServerSocket);
		WSACleanup();
		return 0;
	}

	print(ofs, "Connection Established");
	//open file to recieve char from client
	ofstream fout("temp.txt");
	//check if file open
	if (!fout) {
		print(ofs, "Cannot open file for write");
	}
	else {
		int length = sizeof(packet);
		while (length == sizeof(packet)) {
			//receives RxBuffer
			char RxBuffer[128] = {};
			//recieve char from client
			length = recv(ConnectionSocket, RxBuffer, sizeof(packet), 0);
			packet p;
			if (length == sizeof(packet)) {
				memcpy(&p, RxBuffer, sizeof(packet));
				//store data from packet
				for (int i = 0; i < p.length; i++) {
					char c = p.data[i];
					fout.put(c);
					RxBuffer[0] = c;
					RxBuffer[1] = '\0';
					print(ofs, RxBuffer);
				}
			}
		}
		fout.close(); //close output file
	}
	closesocket(ConnectionSocket);	//closes incoming socket
	closesocket(ServerSocket);	    //closes server socket	
	WSACleanup();					//frees Winsock resources

	return 1;
}
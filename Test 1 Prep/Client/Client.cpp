#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <fstream>
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
	std::ofstream ofs("Client_Output.txt");
	if (!ofs.is_open())
		std::cout << "ERROR:  Failed to open the output file" << std::endl;

	//starts Winsock DLLs
	WSADATA wsaData;
	if ((WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
		return 0;
	}

	//initializes socket. SOCK_STREAM: TCP
	SOCKET ClientSocket;
	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ClientSocket == INVALID_SOCKET) {
		WSACleanup();
		print(ofs, "ERROR:  Failed to create ClientSocket");
		return 0;
	}

	//Connect() socket to specified server
	sockaddr_in SvrAddr;
	SvrAddr.sin_family = AF_INET;						//Address family type itnernet
	SvrAddr.sin_port = htons(27000);					//port (host to network conversion)
	SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");	//IP address
	if ((connect(ClientSocket, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR) {
		closesocket(ClientSocket);
		WSACleanup();
		print(ofs, "ERROR:  Connection attempted failed");
		return 0;
	}


	//receives Rxbuffer

	//while (1) {
		//sends Txbuffer	
		char filename[128] = {};
		char TxBuffer[128] = {};
		//ask for filename
		print(ofs, "Enter a filename");
		cin.getline(filename, 128);
		//open file
		ifstream fin(filename);
		//check if file opens
		if (!fin) {
			print(ofs, "Cannot open file");
		}
		//send file to server
		else {
			packet p;
			int ch = fin.get(); //get char from file
			int packetNumber = 1;
			//loop to end of file
			while (ch != -1){
			int i = 0;

			while (ch != -1 && i < 100) {
				cout << ch;
				p.data[i] = ch;
				i++;
				ch = fin.get();
				
			}
				//put data in packet
				p.header = packetNumber;
				p.srcdest = 0x01;
				p.length = i;
				p.data[i] = '\0';
				//fill TXbuffer with packet
				memcpy(TxBuffer, &p, sizeof(packet));
		//		TxBuffer[0] = ch; //put char in transmit buffer
			//	TxBuffer[1] = '\0';
				//send transmit buffer to server
				send(ClientSocket, TxBuffer, sizeof(packet), 0);
				ch = fin.get(); //get next char from file
			}
			//send null byte to server to close file
			//TxBuffer[0] = '\0';
			send(ClientSocket, TxBuffer, sizeof(TxBuffer), 0);
			fin.close(); //close file
		}
	//}

	//closes connection and socket
	closesocket(ClientSocket);
	//frees Winsock DLL resources
	WSACleanup();

	return 1;
}
#include "oop_winsock.h"
#include <iostream>
#include <fstream>
#include <thread>
void Print(std::string msg, std::ofstream *ofs)
{
	std::cout << msg << std::endl;
	*ofs << msg << std::endl;
}

int main(int argc, char *argv[]) {
	std::string msgToWrite;
	std::ofstream ofs(argv[1]);
	if (!ofs.is_open())
		std::cout << "ERROR:  Failed to open output file -- screen shots will be required" << std::endl;

	winsock_client client(27000, "127.0.0.1", &ofs);
	winsock_client client2(27001 , "127.0.0.1", &ofs);

	client.connect_to_tcp_server_loop();
	client2.connect_to_tcp_server_loop();

	std::thread t1, t2;
	t1 = std::thread(&winsock_client::get_messages, &client);
	t2 = std::thread(&winsock_client::get_messages, &client2);

	t1.join();
	t2.join();



	/*
	char rx_message[128] = "", tx_message[128] = "Hello";

	strcpy(rx_message, client.receive_message());
	std::string s(rx_message, strlen(rx_message));
	msgToWrite = "server message:" + s;
	Print(msgToWrite, &ofs);



	if (strcmp(rx_message, "Server Full") == 0) {
		Print("Could not connect to server", &ofs);
		std::cin.get();
	} else {
		while (true) {
			Print("Enter message to send: ", &ofs);
			std::cin.getline(tx_message, sizeof(tx_message));
			client.send_message(tx_message);
			strcpy(rx_message, client.receive_message());
			std::string s(rx_message, strlen(rx_message));
			msgToWrite = "server ack:" + s;
			Print(msgToWrite, &ofs);
			if (strcmp(tx_message, "quit") == 0)
				break;
		}
	}
	*/

	WSACleanup();
	exit(0);
}
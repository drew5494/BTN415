#include "oop_winsock.h"
#include <iostream>
#include <fstream>

extern int const MAX_SOCKETS;

int main() {
	std::ofstream ofs1("Server1_Output.txt");
	if (!ofs1.is_open())
		std::cout << "ERROR:  Failed to open output file -- Screenshots will be required" << std::endl;

	winsock_server server(27000, "127.0.0.1", &ofs1);
	winsock_server server2(27001, "127.0.0.1", &ofs1);

	int socket_number;
	int socket_number2;

		socket_number = server.accept_connection();
		if ( socket_number < MAX_SOCKETS) {
			std::thread(&winsock_server::echo_mode, &server, socket_number).detach();
		}
	}

		socket_number2 = server.accept_connection();
		if (socket_number2 < MAX_SOCKETS) {
			std::thread(&winsock_server::echo_mode, &server, socket_number2).detach();
		}

	std::thread t1, t2;
	t1 = std::thread(&winsock_server::send_mode, &server, socket_number);
	t2 = std::thread(&winsock_server::rec_mode, &server, socket_number2);

	t1.join();
	t2.join();

	WSACleanup();
	exit(0);
}

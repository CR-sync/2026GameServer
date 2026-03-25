#include <iostream>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

constexpr short SERVER_PORT = 3000;
constexpr int BUFFER_SIZE = 4000;

int main()
{
	std::wcout.imbue(std::locale("korean"));
	WSADATA wsa_data{};
	WSAStartup(MAKEWORD(2, 2), &wsa_data);
	SOCKET s_socket(WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, 0));
	SOCKADDR_IN server_addr{};
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(s_socket, reinterpret_cast<SOCKADDR*>(&server_addr), sizeof(server_addr));
	listen(s_socket, SOMAXCONN);
	INT addr_len = sizeof(server_addr);
	SOCKET c_socket = WSAAccept(s_socket, reinterpret_cast<SOCKADDR*>(&server_addr), &addr_len, nullptr, 0);


	int playerX{ 4 }, playerY{ 4 };
	for (;;) {
		char recv_buffer[BUFFER_SIZE]{};
		WSABUF recv_wsa_buf{ BUFFER_SIZE, recv_buffer };
		DWORD recv_size = 0;
		DWORD rec_flag = 0;
		int result = WSARecv(c_socket, &recv_wsa_buf, 1, &recv_size, &rec_flag, nullptr, nullptr);

		if (result == SOCKET_ERROR){
			break;
		}
		recv_buffer[recv_size] = '\0';

		std::cout << "Received from client: " << recv_buffer << std::endl;


		if (strcmp(recv_buffer, "KEY LEFT") == 0) {
			if (playerX > 0) playerX--;
		}
		else if (strcmp(recv_buffer, "KEY RIGHT") == 0) {
			if (playerX < 7) playerX++;
		}
		else if (strcmp(recv_buffer, "KEY UP") == 0) {
			if (playerY > 0) playerY--;
		}
		else if (strcmp(recv_buffer, "KEY DOWN") == 0) {
			if (playerY < 7) playerY++;
		}

		char send_buffer[BUFFER_SIZE]{};
		sprintf_s(send_buffer, "POS %d %d", playerX, playerY);

		WSABUF send_wsa_buf{static_cast<ULONG>(strlen(send_buffer)) + 1, send_buffer};

		DWORD sent_size = 0;
		WSASend(c_socket, &send_wsa_buf, 1, &sent_size, 0, nullptr, nullptr);

		std::cout << "Sent to client: " << '\n';
	}
	WSACleanup();
}
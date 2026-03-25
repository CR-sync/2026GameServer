#include <iostream>
#include <WS2tcpip.h>	//윈도우 소켓 라이브러리
#include <conio.h>

#pragma comment(lib, "ws2_32.lib")

constexpr short SERVER_PORT = 3000;
constexpr int BUFFER_SIZE = 4000;
constexpr int boardSize = 8;

// 보드관련 함수
char Board[boardSize][boardSize];
void initBoard();
void printBoard();

void error_display(const wchar_t* msg, int err_no)
{
	WCHAR* lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	std::wcout << msg;
	std::wcout << L" === 에러 " << lpMsgBuf << std::endl;
	while (true); // 디버깅 용
	LocalFree(lpMsgBuf);
}

int main()
{
	std::string serverIP;
	std::cout << "Enter Server IP: ";
	std::cin >> serverIP;

	std::wcout.imbue(std::locale("korean"));
	WSADATA wsa_data{};
	WSAStartup(MAKEWORD(2, 2), &wsa_data);
	SOCKET s_socket(WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, 0));
	SOCKADDR_IN server_addr{};
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, serverIP.c_str(), &server_addr.sin_addr);
	int result = WSAConnect(s_socket, reinterpret_cast<SOCKADDR*>(&server_addr), sizeof(server_addr), nullptr, nullptr, nullptr, nullptr);	//서버에 연결
	if (result == SOCKET_ERROR) {
		error_display(L"서버 연결 실패", WSAGetLastError());
		return 1;
	}

	for (;;) {
		int key = _getch();
		std::string sendMsg;

		switch (key) {
		case 75:
			sendMsg = "KEY LEFT";
			break;
		case 77:
			sendMsg = "KEY RIGHT";
			break;
		case 72:
			sendMsg = "KEY UP";
			break;
		case 80:
			sendMsg = "KEY DOWN";
			break;
		default:
			continue;
		}

		char buffer[BUFFER_SIZE]{};
		std::string input;
		std::cout << "Enter message to send: ";
		std::cin.getline(buffer, BUFFER_SIZE);

		WSABUF wsa_buf{ static_cast<ULONG>(strlen(buffer)) + 1, buffer };
		DWORD sent_size = 0;
		int result = WSASend(s_socket, &wsa_buf, 1, &sent_size, 0, nullptr, nullptr);

		if (result == SOCKET_ERROR) {
			error_display(L"데이터 전송 실패", WSAGetLastError());
			return 1;
		}

		char recv_buffer[BUFFER_SIZE]{};	//수신 버퍼
		WSABUF recv_wsa_buf{ BUFFER_SIZE, recv_buffer };
		DWORD recv_size = 0;
		DWORD rec_flag = 0;
		WSARecv(s_socket, &recv_wsa_buf, 1, &recv_size, &rec_flag, nullptr, nullptr);
		std::cout << "Received from server: " << recv_buffer << std::endl;
		std::cout << "SIZE: " << recv_size << std::endl;
	}
	WSACleanup();
}

void initBoard()
{
	for (int i = 0; i < boardSize; ++i) {
		for (int j = 0; j < boardSize; ++j) {
			Board[j][i] = ' ';
		}
	}
}

void printBoard()
{
	for (int i = 0; i < boardSize; ++i)
	{
		std::cout << "---------------------------------" << '\n';
		for (int j = 0; j < boardSize; ++j)
		{
			std::cout << "| " << Board[j][i] << ' ';
		}
		std::cout << '|' << '\n';
	}
	std::cout << "---------------------------------" << '\n';
}
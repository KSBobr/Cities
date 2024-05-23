#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <windows.h>
#pragma warning(disable: 4996)

SOCKET Connection;
int serv_ok=1;
void ClientHandler() {
	char msg[256];
	while (true) {
		//std::cout << recv(Connection, msg, sizeof(msg), NULL)<<'\n';
		if (recv(Connection, msg, sizeof(msg), NULL)>0)
		{
			std::cout << msg << std::endl;
		}
		else { std::cout << "disconnected\n"; serv_ok = 0; return; }
	}
}

int main(int argc, char* argv[]) {
	//WSAStartup
	SetConsoleCP(1251);
	setlocale(LC_ALL, "rus");
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		std::cout << "Error: failed connect to server.\n";
		return 1;
	}
	std::cout << "Connected!\n";

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);

	char msg1[256];
	while (serv_ok) {
		std::cin.getline(msg1, sizeof(msg1));
		send(Connection, msg1, sizeof(msg1), NULL);
		Sleep(10);
	}
	std::cout << "Извините сервер упал :(, обратитесь к разрабам они пофиксят (если захотят) и вы сможете перезайти";
	//system("pause");
	return 0;
}
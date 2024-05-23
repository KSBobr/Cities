//#define CRT_SECURE_NO_WaRNINGS;
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <vector>
#include<stdio.h>
#include <string>
#include <map>
#include<windows.h>
#include <fstream>
#pragma warning(disable: 4996)
void Stringtocharm(std::string s,char* mc)
{
	
	for (int i = s.length(); i < 256; i++)
	{
		mc[i] = '\0';
	}
	for (int j = 0; j < s.length();j++)
	{
		mc[j] = s[j];
	}
	
}
std::map <std::string, bool> CBase;
SOCKET Connections[1000];
int Counter = 0;
int FFI = 0; 
int FFIS = -1;
std::vector <SOCKET> Lobbies[1000];
std::vector <short int> LR[1000];
void StartGame(int ind)
{	
	int offline_players;
	std::map <std::string, bool>Proverka;
	//std::cout << Lobbies[ind].size() << '\n';
	for (int i = 0; i < Lobbies[ind].size(); i++)
	{
		char msg[256] = "Вы зашли в лобби, начинает хост с города на букву А!";
		//std::cout << i << '\n';
		send(Lobbies[ind][i], msg, sizeof(msg), NULL);
	}
	char ll = 'А';
	while (Lobbies[ind].size()>0)
	{
		std::cout << "hui\n";
		for (int i = 0; i < Lobbies[ind].size();i++)
		{
			int unnormal_city=1;
			while (unnormal_city)
			{
				char msg[256];
				if (recv(Lobbies[ind][i], msg, sizeof(msg), NULL) > 0)
				{
					if (CBase[msg])
					{
						//std::cout << ll << ' ' << msg[0] <<'\n';
						if (msg[0] == ll)
						{
							if (!Proverka[msg])
							{
								Proverka[msg] = 1;
								/*char msgs[256] = "Этот город подходит!";
								send(Lobbies[ind][i], msgs, sizeof(msgs), NULL);*/
								for (int j = 0; j < Lobbies[ind].size(); j++)
								{
									if (i != j)
									{
										send(Lobbies[ind][j], msg, sizeof(msg), NULL);
									}
								}
								std::string msg_cost = msg;
								//std::cout << msg_cost;
								ll = std::toupper(msg_cost[msg_cost.length() - 1]);
								int nonb = 2;
								while ((ll == 'Ъ' or ll == 'Ь' or ll == 'Ы') and msg_cost.length() - nonb > 0)
								{
									ll = std::toupper(msg_cost[msg_cost.length() - nonb]);
								}
								std::string sss = "Твой ход, тебе на букву "; sss += ll;
								//std::cout <<sss;
								char msgs2[256];
								Stringtocharm(sss, msgs2);
								send(Lobbies[ind][(i + 1) % Lobbies[ind].size()], msgs2, sizeof(msgs2), NULL);
								//std::cout << ' ' << ll << '\n';
								unnormal_city = 0;
							}
							else
							{
								char msgs[256] = "Этот город уже был!";
								send(Lobbies[ind][i], msgs, sizeof(msgs), NULL);
							}
						}
						else
						{
							char msgs[256] = "Этот город не подходит!";
							send(Lobbies[ind][i], msgs, sizeof(msgs), NULL);
						}
					}
					else
					{
						char msgs[256] = "Нет такого города";
						send(Lobbies[ind][i], msgs, sizeof(msgs), NULL);
					}
				}
				else
				{	
					std::cout << "Отваливься\n";
					Lobbies[ind].erase((Lobbies[ind].begin()+i));
					i--;
					break;
				}
			}
		}
	}
	//std::cout << ll;
}
void createLobby(SOCKET a)
{
	Lobbies[FFI].push_back(a);
	Counter++;
}
void ClientHandler(int index) {
	char msgs[256];
	recv(Connections[index], msgs, sizeof(msgs), NULL);
	if (msgs[0] == '1')
	{
		int kost = FFI;
		createLobby(Connections[index]);
		FFI++;
		char msg[256] = "Press to start game";
		send(Connections[index], msg, sizeof(msg), NULL);
		recv(Connections[index], msg, sizeof(msg), NULL);
		std::cout << "try to create lobby\n";
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)StartGame, (LPVOID)(kost), NULL, NULL);
	}
	else
	{
		if (Counter == 0)
		{
			char mssss[256] = "No lobbies(";
			send(Connections[index], mssss, sizeof(mssss), NULL);
			recv(Connections[index], msgs, sizeof(msgs), NULL);
		}
		else
		{
			char mssss[256] = "List of lobbyies, choise number:";
			send(Connections[index], mssss, sizeof(mssss), NULL);
			recv(Connections[index], msgs, sizeof(msgs), NULL);
			std::string s = msgs;
			Lobbies[std::stoi(s)].push_back(Connections[index]);
		}
	}
	Counter++;
}

int main(int argc, char* argv[]) {
	//WSAStartup;
	SetConsoleCP(1251);
	setlocale(LC_ALL, "rus");
	std::ifstream in;
	in.open("city_base.txt");
	std::string cityf;
	if (in.is_open())
	{
		while (std::getline(in, cityf))
		{

			//std::cout << cityf << std::endl;
			CBase[cityf] = 1;
		}
	}
	in.close();
	/*std::string aboba;
	std::getline(std::cin,aboba);
	std::cout <<aboba << CBase[aboba];*/
	//std::cout << "hyi";
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

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;
	while (true) {
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

		if (newConnection == 0) {
			std::cout << "Error #2\n";
		}
		else {
			std::cout << "Client Connected!\n";
			char msg[256] = "game goroda!1-createLobby,2-joinLobby";
			send(newConnection, msg, sizeof(msg), NULL);
			FFIS++; 
			Connections[FFIS] = newConnection;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(FFIS), NULL, NULL);
		}
	}
	system("pause");
	return 0;
}
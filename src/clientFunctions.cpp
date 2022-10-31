#include "clientFunctions.h"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

void Client::eraseText(int cnt)
{
	char back_space = 8;
	for (int i = 0; i < cnt; i++)
	{
		std::cout << back_space;
	}
}

void Client::sendMessage(int clientSocket)
{
	while (1)
	{
		int checkIfSent;
		std::cout << "You: ";
		char str[MAX_LEN];
		std::cin.getline(str, MAX_LEN);
		checkIfSent = send(clientSocket, str, sizeof(str), 0);
		
		if (strcmp(str, "#exit") == 0)
		{
			exit_flag = true;
			t_recv.detach();
			close(clientSocket);
			return;
		}
		if (checkIfSent != -1)
		{
			std::cout << "Message Delivered...\n";
		}
	}
}

void Client::recieveMessage(int clientSocket)
{
	while (1)
	{
		if (exit_flag)
		{
			return;
		}
		char name[MAX_LEN], str[MAX_LEN];
		int bytes_received = recv(clientSocket, name, sizeof(name), 0);
		if (bytes_received <= 0)
		{
			continue;
		}

		recv(clientSocket, str, sizeof(str), 0);
		eraseText(6);
		if (strcmp(name, "#NULL") != 0)
		{
			std::cout << name << " : " << str << std::endl;
		}
		else
		{
			std::cout << str << std::endl;
		}
		std::cout << "You : ";
		fflush(stdout);
	}
}
#include <serverFunctions.h>
#include <cstring>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAX_LEN 200

void Server::setName(int id, char name[])
{
	for (int i = 0; i < clients.size(); i++)
	{
		if (clients[i].id == id)
		{
			clients[i].name = std::string(name);
		}
	}
}

void Server::sharedPrint(std::string str, bool endLine = true)
{
	std::lock_guard<std::mutex> guard(cout_mtx);
	std::cout << str;
	if (endLine)
		std::cout << std::endl;

	//Write in file
	std::ofstream myFile;
	myFile.open("ChatRecord.txt", std::ios_base::app);
	myFile << str << std::endl;
	myFile.close();
}

void Server::broadcastMessage(std::string message, int sender_id)
{
	char temp[MAX_LEN];
	strcpy(temp, message.c_str());
	for (int i = 0; i < clients.size(); i++)
	{
		if (clients[i].id != sender_id)
		{
			send(clients[i].socket, temp, sizeof(temp), 0);
		}
	}
}

void Server::endConnection(int id)
{
	for (int i = 0; i < clients.size(); i++)
	{
		if (clients[i].id == id)
		{
			std::lock_guard<std::mutex> guard(clients_mtx);
			clients[i].th.detach();
			clients.erase(clients.begin() + i);
			close(clients[i].socket);
			break;
		}
	}
}

void Server::handleClient(int clientSocket, int id)
{
	char name[MAX_LEN], str[MAX_LEN];
	recv(clientSocket, name, sizeof(name), 0);
	setName(id, name);

	// Display welcome message
	std::string welcomeMessage = std::string(name) + std::string(" has joined");
	broadcastMessage("#NULL", id);
	broadcastMessage(welcomeMessage, id);
	sharedPrint(welcomeMessage);

	while (1)
	{
		int bytesReceived = recv(clientSocket, str, sizeof(str), 0);
		if (bytesReceived <= 0)
			return;
		if (strcmp(str, "#exit") == 0)
		{
			// Display leaving message
			std::string message = std::string(name) + std::string(" has left");
			broadcastMessage("#NULL", id);
			broadcastMessage(message, id);
			sharedPrint(message);
			endConnection(id);
			return;
		}
		broadcastMessage(std::string(name), id);
		broadcastMessage(std::string(str), id);
		sharedPrint(std::string(name) + " : " + str);
	}
}

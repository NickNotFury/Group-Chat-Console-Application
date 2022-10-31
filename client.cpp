#include "clientFunctions.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <cstring>

int main()
{
	Client cl;
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket < 0)
	{
		std::cout << "Error: Couldn't Open Socket\n";
		exit(EXIT_FAILURE);
	}
	struct sockaddr_in client_addr;
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(10000);
	client_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(client_addr.sin_zero), 0);

	if (connect(clientSocket, (struct sockaddr *)&client_addr, sizeof(struct sockaddr_in)) < 0)
	{
		std::cout << "Error: Connection Failed.\n";
		exit(EXIT_FAILURE);
	}

	char name[MAX_LEN];
	std::cout << "Enter your name : ";
	std::cin.getline(name, MAX_LEN);

	send(clientSocket, name, sizeof(name), 0);

	std::cout << "----------Group Chat Application----------(Enter '#exit' to exit the group)" << std::endl;

	//Show previous chats when user joins
	std::string myText;
	std::ifstream myReadFile;
	myReadFile.open("ChatRecord.txt");
	if (myReadFile.is_open())
		std::cout << myReadFile.rdbuf();

	myReadFile.close();

	std::thread t1(&Client::sendMessage, &cl, clientSocket);
	std::thread t2(&Client::recieveMessage, &cl, clientSocket);

	t_send = move(t1);
	t_recv = move(t2);

	if (t_send.joinable())
		t_send.join();
	if (t_recv.joinable())
		t_recv.join();

	return 0;
}
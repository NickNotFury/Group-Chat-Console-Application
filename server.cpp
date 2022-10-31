#include "serverFunctions.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <signal.h>

int main()
{   
    Server serv;
    int clientId = 0;
    int nRet = 0;

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket < 0)
    {
        std::cout << "Error: Couldn't Open Socket\n";
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(10000);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&serv_addr.sin_zero, 0);

    nRet = bind(serverSocket, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in));
    if (nRet < 0)
    {
        std::cout << "Error: Failed to bind to local port\n";
        exit(EXIT_FAILURE);
    }

    nRet = listen(serverSocket, 8);
    if (nRet < 0)
    {
        std::cout << "Error: Failed to start listen to local port\n";
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in client_addr;
    int clientSocket;
    unsigned int len = sizeof(sockaddr_in);

    std::cout << "----------Group Chat Application----------" << std::endl;
    while (1)
    {
        clientSocket = accept(serverSocket, (struct sockaddr *)&client_addr, &len);
        if (clientSocket < 0)
        {
            std::cout << "Accept Error\n";
            exit(EXIT_FAILURE);
        }
        clientId++;

        std::thread t(&Server::handleClient, &serv, clientSocket, clientId);
        std::lock_guard<std::mutex> guard(clients_mtx);
        clients.push_back({clientId, std::string("Anonymous"), clientSocket, (move(t))});
    }

    for (int i = 0; i < clients.size(); i++)
    {
        if (clients[i].th.joinable())
            clients[i].th.join();
    }

    close(serverSocket);
    return 0;
}
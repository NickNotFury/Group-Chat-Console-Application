#ifndef SERVERFUNCTIONS_H
#define SERVERFUNCTIONS_H

#include <iostream>
#include<fstream>
#include <thread>
#include <mutex>
#include <vector>

struct clientInfo
{
    int id;
    std::string name;
    int socket;
    std::thread th;
};

std::mutex cout_mtx, clients_mtx;
std::vector<clientInfo> clients;

class Server
{
public:
    void setName(int id, char name[]);
    void sharedPrint(std::string, bool);
    void broadcastMessage(std::string, int);
    void broadcastMessage(int, int);
    void endConnection(int);
    void handleClient(int, int);
};

#endif
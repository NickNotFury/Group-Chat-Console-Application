#ifndef CLIENTFUNCTIONS_H
#define CLIENTFUNCTIONS_H

#include <thread>
#define MAX_LEN 200

std::thread t_send, t_recv;

class Client
{
private:
    int clientSocket;
    bool exit_flag = false;

public:
    void eraseText(int cnt);
    void sendMessage(int clientSocket);
    void recieveMessage(int clientSocket);
};

#endif
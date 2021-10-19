#ifndef NODECONNECTION_H
#define NODECONNECTION_H

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <iostream>
#include <mutex>
#include <thread>
#define PORT 8080

class nodeConnection
{
private:
    char* ip;

    char buffer[1024] = {0};

    std::mutex mtx;

    std::thread *pThread = nullptr;
public:
    nodeConnection(char* ip);
    ~nodeConnection();

    void connectToNode();

    void startServer();

    std::string getConnection();

};

#endif // NODECONNECTION_H

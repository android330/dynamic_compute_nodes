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
    const char* ip;

    char buffer[1024] = {0};

    std::mutex mtx;

    std::thread *pThread = nullptr;

    char message[1024] = {0};


    void connectToNode();
    void startServer();
public:
    nodeConnection(const char* ip);
    ~nodeConnection();


    std::string getConnection();
    void setMessage(char msg[]);

};

#endif // NODECONNECTION_H

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
#include <netdb.h> //Hostname Recognition


class nodeConnection
{
private:
    const char* ip;

    int port;

    int* send_socket = nullptr;

    char buffer[1024] = {0};

    std::mutex mtx;

    std::thread *pThread = nullptr;

    char message[1024] = {0};

    std::string readString = "9";

    void connectToNode();
    void startServer();
public:
    nodeConnection(const char* ip, const int port);
    ~nodeConnection();


    std::string getConnection();
    void setMessage(const char msg[]);

    void sendMessage();

};

#endif // NODECONNECTION_H

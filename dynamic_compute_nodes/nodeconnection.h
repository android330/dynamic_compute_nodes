/*
    nodeconnection.h
    Andrew Beachemin
    android@vt.edu
    October 2021
    The node class controls connects to a node, and if cant connection, listens for connection
*/

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

    //connect to node calls start server, and start server calls connect to node
    void connectToNode();
    void startServer();
public:
    //constructor and destructor
    nodeConnection(const char* ip, const int port);
    ~nodeConnection();

    //gets what is in recieve buffer
    std::string getConnection();

    //sets message in send buffer
    void setMessage(const char msg[]);

    //sends message
    void sendMessage();
};

#endif // NODECONNECTION_H

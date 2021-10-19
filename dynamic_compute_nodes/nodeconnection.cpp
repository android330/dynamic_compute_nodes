#include "nodeconnection.h"

nodeConnection::nodeConnection(char* ip)
{
    this->ip = ip;
    pThread = new std::thread(&nodeConnection::connectToNode, this);
}

nodeConnection::~nodeConnection(){
    if(pThread->joinable())
        pThread->join();
    delete pThread;
    delete ip;
}

void nodeConnection::connectToNode(){
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = (char*) "Hello from client";

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return;
    }

    //if the client module is unable to connect, it starts a server module for the client on another node to connect to
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        startServer();
        return;
    }

    while(true){
        send(sock , hello , strlen(hello) , 0 );
        printf("Hello message sent\n");
        mtx.try_lock();
        valread = read( sock , buffer, 1024);
        mtx.unlock();
        printf("%s\n",buffer );
        std::cin.ignore();
    }
    return;


}

void  nodeConnection::startServer(){
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    char *hello = (char*) "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    while(true){
        mtx.try_lock();
        valread = read( new_socket , buffer, 1024);
        mtx.unlock();
        printf("%s\n",buffer );
        send(new_socket , hello , strlen(hello) , 0 );
        printf("Hello message sent\n");
    }
    return;
}

std::string nodeConnection::getConnection(){
    mtx.try_lock();
    std::string temp = buffer;
    mtx.unlock();
    return temp;
}

/*
int main(int argc, char const *argv[]){
    nodeConnection a((char*)"127.0.0.1");
}
*/


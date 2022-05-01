/*
    nodeconnection.h
    Andrew Beachemin
    android@vt.edu
    October 2021
*/

#include "nodeconnection.h"

nodeConnection::nodeConnection(const char* ip, const int port)
{
    this->ip = ip;
    this->port = port;

    //starts connects to node as new thread
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

    ///sock initilization test
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return;
    }

    //assumes IPv4, and sets port
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    //Checks if the first character of the ip alphabatical, if it is then it assuems that it is a hostname
    if(isalpha(ip[0]))
    {
        //resolves hostname
        struct hostent *hostname;
        hostname = gethostbyname(ip);
        if(hostname == NULL)
        {
            std::cout << (std::string) ip;
	    printf(" - Not Online\n");
            return;
        }
        else
        {
	    std::cout << (std::string) ip << " - Resolved to ";
            //sets new ip to the resolved hostname if it was valid
            this->ip = inet_ntoa(*((struct in_addr *) hostname->h_addr_list[0]));
	    std::cout << (std::string) ip << std::endl;
        }
    }

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

    //recieves messages whle loop, blocking
    send_socket = &sock;
    while(true){
        memset(&buffer[0], 0, sizeof(buffer));
        valread = read( sock , buffer, 1024);

        if (valread)
        {
            readString = buffer;
        }

        //if connection close, start server
        else 
        {
            if (shutdown(sock, SHUT_RDWR))
                return;
            close(sock);
            startServer();
        }
    }
    return;


}

void  nodeConnection::startServer(){
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( port );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    //listens
    if (listen(server_fd, 3) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //accept connections on correct socket
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0){
        perror("accept");
        exit(EXIT_FAILURE);
    }

    //blocking while loop for recieving data
    send_socket = &new_socket;
    while(true){
        memset(&buffer[0], 0, sizeof(buffer));
        valread = read( new_socket , buffer, 1024);

        if (valread)
        {
            readString = buffer;
        }

        //if connection close transition to server
        else 
        {
            if (shutdown(new_socket, SHUT_RDWR))
                return;
            close(new_socket);
            startServer();
        }
    }
    return;
}

std::string nodeConnection::getConnection(){
    //read string then wipe varaible
    std::string temp = readString;
    readString = "";
    return temp;
}

void nodeConnection::setMessage(const char* msg){
    //wipe memory, then copy string
    memset(&message[0], 0, sizeof(message));
    strcpy(message, msg);
}

void nodeConnection::sendMessage(){
    //send string then wipe memory
    if (send_socket != nullptr)
        send(*send_socket , message , strlen(message) , 0 );
    memset(&message[0], 0, sizeof(message));
}

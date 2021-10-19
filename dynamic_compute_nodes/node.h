#ifndef NODE_H
#define NODE_H

#include "nodeconnection.h"
#include <vector>
#include <thread>
#include <time.h>

#define CONNECTION_QUANTITY 1

class node
{
private:
    //add more ips to vector to connect to more nodes
    const char *IPS[CONNECTION_QUANTITY] = {"127.0.0.1"};

    std::vector<nodeConnection*> connections;
public:
    node();
    ~node();
};

#endif // NODE_H

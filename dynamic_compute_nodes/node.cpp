#include "node.h"

node::node()
{
    nodeConnection* temp;
    for (int i = 0; i < CONNECTION_QUANTITY; i++){
        temp = new nodeConnection(IPS[i]);
        connections.push_back(temp);
    }

    while(true){
        usleep(10000000);
        std::cout << ">>>   Stuff in TCP buffer: " << connections.at(0)->getConnection() << std::endl;
        char buf[sizeof(int)*3+2];
        srand(time(NULL));
        snprintf(buf, sizeof buf, "%d", rand()%10000 + 1);
        std::cout << ">>>   Set Message to: " << buf << std::endl;
        connections.at(0)->setMessage(buf);

    }



    //nodeConnection e((char*)"127.0.0.1");

    //std::thread t (e.connectToNode());

    //nodeConnection a((char*)"127.0.0.1");
    //nodeConnection nodeCon((char*)"127.0.0.1");

    //std::thread t(&nodeConnection::connectToNode, this);

    //t.join();

    //t.join();
}

node::~node(){

}

int main(int argc, char const *argv[]){
    node a;

}

#include "nodeconnection.h"
#include <vector>
#include <thread>
#include <time.h>
#include <limits.h>
#include <stdlib.h>
#include "job.hpp"

#ifdef __arm__
#include <wiringPi.h>
#endif


#define CONNECTION_QUANTITY 4

#define SEEKINGCONNECTION_ 0
#define ACCEPTEDCONNECTION_ 1
#define DATASEND_ 2
#define CLOSE_ 3


class Node_Context;

class Node_State
{
protected:
    Node_Context *context_;

public:
    virtual ~Node_State(){}

    void set_context(Node_Context *context)
    {
        this->context_ = context;
    }

    virtual void onRun() = 0;
};

class Node_Context
{
private:
    Node_State* state_;
public:
    const char *IPS[CONNECTION_QUANTITY] = {"127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1"};
    const int PORTS[CONNECTION_QUANTITY] = {8080, 8080, 8080, 8080};

    const char *TERMINAL_IP = "127.0.0.1";
    nodeConnection* terminalConnection;

    std::vector<nodeConnection*> connections;

    //char hostname[1024];
    //gethostname(hostname, 1024);
    std::string username;


    Job* job;

    Node_Context(Node_State* state, const char* ip[]) : state_(nullptr)
    {
        this->TransitionTo(state);
        //IPS[0] = ip;
        std::vector<std::string> hostnames = { "azimov", "bachman", "card", "douglas", "egg" };

        username = ip[1];

        int z = 0;
        for (int i = 0; i < CONNECTION_QUANTITY + 1; i++) {
            if (ip[1] == hostnames.at(i)) {
                continue;
            }
            std::cout << (hostnames.at(i) + ".local").c_str() << std::endl;
            IPS[z] = (hostnames.at(i) + ".local").c_str();
            z++;
        }
        TERMINAL_IP = ip[2];
    }

    ~Node_Context(){delete state_;}

    void TransitionTo(Node_State *state)
    {
        if (this->state_ != nullptr)
            delete this->state_;

        this->state_ = state;
        this->state_->set_context(this);
//	std::cout << this->job->name << std::endl;
    }

    void start()
    {
        this->state_->onRun();
    }
};


class Idle : public Node_State{
private:
    bool startup = false;
    clock_t start;
public:
    void onRun() override;};

class Operate : public Node_State{
private:
    bool startup = false;
    clock_t start;
public:
    void onRun() override;
};

class DataSend : public Node_State{void onRun() override;};
class Stop : public Node_State{void onRun() override;};

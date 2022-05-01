/*
    node.h
    Andrew Beachemin
    android@vt.edu
    October 2021
    The node class controls which state the machine is in (see state diagram for more information)
*/

#include "nodeconnection.h"
#include <vector>
#include <thread>
#include <time.h>
#include <limits.h>
#include <stdlib.h>
#include "job.hpp"

//only tries to use wiring pi if on ARM machine
#ifdef __arm__
#include <wiringPi.h>
#endif


#define CONNECTION_QUANTITY 4

//defines for state
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

    //adjacent hosts
    const char *hostnames[CONNECTION_QUANTITY + 1] = {"azimov.local", "bachman.local", "card.local", "douglas.local", "egg.local"};
public:
    const char *IPS[CONNECTION_QUANTITY] = {"127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1"};
    const int PORTS[CONNECTION_QUANTITY] = {8080, 8080, 8080, 8080};
    const char *TERMINAL_IP = "127.0.0.1";

    nodeConnection* terminalConnection;
    std::vector<nodeConnection*> connections;
    std::string username;
    Job* job;

    //node context constructor
    Node_Context(Node_State* state, const char* ip[]) : state_(nullptr)
    {
        this->TransitionTo(state);

        username = ip[1];

        //loops through all connections to figure out which one is the current connection so that it doesn't try to connect to itself
        int z = 0;
        for (int i = 0; i < CONNECTION_QUANTITY + 1; i++) {
            if ((std::string) ip[1] == ((std::string) hostnames[i]).substr(0, strlen(hostnames[i]) - 6)) {
                continue;
            }
            IPS[z] = hostnames[i];
            z++;
        }
        TERMINAL_IP = ip[2];
    }

    ~Node_Context(){delete state_;}

    //transiton to funciton for state machine
    void TransitionTo(Node_State *state)
    {
        if (this->state_ != nullptr)
            delete this->state_;

        this->state_ = state;
        this->state_->set_context(this);
    }

    //initilization function
    void start()
    {
        this->state_->onRun();
    }
};

//idle clas header
class Idle : public Node_State
{
private:
    bool startup = false;
    clock_t start;
public:
    void onRun() override;};

//operate class header
class Operate : public Node_State
{
private:
    bool startup = false;
    clock_t start;
public:
    void onRun() override;
};

//datasend and stop class header
class DataSend : public Node_State{void onRun() override;};
class Stop : public Node_State{void onRun() override;};

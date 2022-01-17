#include "nodeconnection.h"
#include <vector>
#include <thread>
#include <time.h>

#ifdef __arm__
#include <wiringPi.h>
#endif


#define CONNECTION_QUANTITY 1

#define ACCEPTEDCONNECTION_ 0
#define DATASEND_ 1
#define CLOSE_ 2

class Job{
public:
    void startRun(){std::cout << "Job Started Running" << std::endl;}
    void endRun(){std::cout << "Job Ended Running" << std::endl;}
    void recievedData(std::string s){std::cout << "Job Recieved: " << s << std::endl;}
    char* send(){std::cout << "Job Data Sent" << std::endl; return (char*) "Job Data";}
};

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
    const char *IPS[CONNECTION_QUANTITY] = {"127.0.0.1"};
    const int PORTS[CONNECTION_QUANTITY] = {8080};

    std::vector<nodeConnection*> connections;

    Job job;





    Node_Context(Node_State* state, const char* ip) : state_(nullptr)
    {
        this->TransitionTo(state);
        IPS[0] = ip;
    }

    ~Node_Context(){delete state_;}

    void TransitionTo(Node_State *state)
    {
        if (this->state_ != nullptr)
            delete this->state_;

        this->state_ = state;
        this->state_->set_context(this);
    }

    void start()
    {
        this->state_->onRun();
    }
};


class Idle : public Node_State{
private:
    bool startup = false;
public:
    void onRun() override;};
class Operate : public Node_State{void onRun() override;};
class DataSend : public Node_State{void onRun() override;};
class Stop : public Node_State{void onRun() override;};

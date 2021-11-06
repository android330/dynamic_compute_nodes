#pragma once

#include <iostream>

struct Args
    {
        bool stop;
        int out;
        Args() : stop(false), out(0){};
    };
class Job
{
private:
    int rpi;
    int hash();
    int pid;
    void* ret;
    Args* args;
    std::string name;
    
public:
    Job(std::string name, int rpi);
    ~Job();
    Job(char* prevJob);

    void startRun();
    void endRun();
    int getVal();
    // char* send();
    std::string toString();
};


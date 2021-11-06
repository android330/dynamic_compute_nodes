#pragma once

#include <iostream>

class Job
{
private:
    int rpi;
    int hash();
public:
    Job(std::string name);
    ~Job();
    Job(char* prevJob);

    void startRun();
    void endRun();
    char* send();
    char* toString();
};


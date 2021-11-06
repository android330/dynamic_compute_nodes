#pragma once

#include <iostream>

class Job
{
private:
    int PI;
    int hash();
public:
    Job(/* args */);
    ~Job();
    Job(char* prevJob);

    void run();
    void stop();
    char* getJob();
};


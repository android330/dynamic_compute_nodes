#pragma once

#include <iostream>
#include <string>

/*
struct Args
    {
        bool stop;
        int out;
        std::string arg;
        Args() : stop(false), out(0){};
    };
*/
class Job
{
public:
    float res;
    int max;
    int pos;
    int count;
    std::string name;
    bool stop;
// public:
    ~Job();
    Job();
    Job(std::string prevJob);

    void monteCarlo();
    void startRun();
    void endRun();
    float getVal();
    std::string send();
    void recievedData(std::string data);
};

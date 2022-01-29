#include "job.hpp"
#include <sstream>
#include <string>
#include <fstream>
#include <cstdio>
#include <spawn.h>
#include <signal.h>
#include <unistd.h>
#include <thread>
#include <mutex>

Job::Job(std::string name, int rpi)
{
    this->rpi = rpi;
    this->name = name;
    
}

Job::~Job()
{
    // delete args;
}

Job::Job(char* prevJob) {
    std::stringstream ss(prevJob);
    getline(ss, name);
    // getline(ss, hash);
    std::string name;
    getline(ss, name);
    // getline(ss, arg_s);
    std::ofstream outputFile;
    // outputFile.open(name);
    // while (ss >> buffer)
    // {
        
    // }
    args->out = 0;
    args->stop = false;
    // args->arg = arg_s;
}

// void *jobLoop (void* arg) {
//     int out = 0;
//     while(!myArgs->stop) {
//         sleep(1);
//         out++;
//     }
//     myArgs->out = out;
//     return nullptr;
// }

void Job::startRun() {
    std::thread thread_obj(run, args);
}

void Job::run(Args* args) {
    
}

void Job::endRun() {
    args->stop = true;
    sleep(2);
    pthread_exit(NULL);
}

int Job::getVal() {
    return args->out;
}

std::string Job::toString() {
    std::string out = "";
    out += name;
    out += " " + rpi;
    out += " " + args->out;
}


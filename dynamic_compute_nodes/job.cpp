#include "job.hpp"
#include <sstream>
#include <string>
#include <fstream>
#include <cstdio>
#include <spawn.h>
#include <signal.h>
#include <unistd.h>

Job::Job(std::string name, int rpi)
{
    this->rpi = rpi;
    this->name = name;
    // this->args = new Args();
}

Job::~Job()
{
    // delete args;
}

Job::Job(char* prevJob) {
    std::stringstream ss(prevJob);
    getline(ss, name);
    getline(ss, hash);
    getline(ss, args);
    getline(ss, cmp);
    std::ofstream outputFile;
    outputFile.open(name);

}

void *jobLoop (void* arg) {
    Args* myArgs = (Args*)arg;
    int out = 0;
    while(!myArgs->stop) {
        sleep(1);
        out++;
    }
    myArgs->out = out;
    return nullptr;
}

void Job::startRun() {
    pthread_t thread;
    pthread_create(&thread, NULL, jobLoop, NULL);
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


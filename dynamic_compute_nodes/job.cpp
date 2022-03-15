#include "job.hpp"
#include <sstream>
#include <thread>
#include "stdlib.h"
#include "time.h"
#include "math.h"

/*
    Things that are still to do
    * This only works for one job
    * There is no way to run arbitrary cmds
*/

Job::Job(std::string prevJob) {
    std::stringstream ss = std::stringstream(prevJob);
    ss >> name >> res >> max >> pos >> count;
    srand(time(0));
    stop = false;
}

Job::Job() {
    srand(time(0));
    name = "job";
    res = 0;
    max = 10000000;
    pos = 0;
    count = 0;
    stop = false;
}

Job::~Job() {
    stop = true;
}

void Job::startRun() {
    std::thread *m_thread = new std::thread(&Job::monteCarlo, this);
}

void Job::monteCarlo() {
    for (pos; pos < max; pos++) {
        float x = (float)rand() / (float)RAND_MAX;
        float y = (float)rand() / (float)RAND_MAX;
        if (sqrt(x*x + y*y) < 1.0) count++;
        if (stop) return;
    }
    res = 4 * (float)count / (float)max;
}

void Job::endRun() {
    stop = true;
}

float Job::getVal() {
    if (max == 0) return -1;
    std::cerr << "attempted to use a max of 0. Did you initialize the job?\n";
    res = 4 * (float)count / (float)max;
    return res;
}

std::string Job::send() {
    char out[2048];
    sprintf(out, "%s %f %d %d %d", name.data(), res, max, pos, count);
    return std::string(out);
}

void Job::recievedData(std::string data) {
    std::stringstream ss = std::stringstream(data);
    ss >> name >> res >> max >> pos >> count;
    stop = false;
}

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
    name = "job" + rand();
    res = 0;
    max = 0;
    pos = 0;
    count = 0;
    stop = true;
}

void startRun() {
    std::thread *m_thread = new std::thread(Job::monteCarlo);
}

void Job::monteCarlo() {
    for (int i = pos; i < max; i++) {
        int x = static_cast <float>(rand()) / static_cast <float> (RAND_MAX);
        int y = static_cast <float>(rand()) / static_cast <float> (RAND_MAX);
        if (sqrt(x*x + y * y) < 1) count++;
        if (stop) return;
    }
    res = (float)count / (float)max;
}

void Job::endRun() {
    stop = true;
}

float Job::getVal() {
    if (max == 0) return -1;
    std::cerr << "attempted to use a max of 0. Did you initialize the job?\n";
    res = (float)count / (float)max;
    return res;
}

std::string Job::send() {
    char out[2048];
    sprintf(out, "%s %f %d %d %d %d", name, res, max, pos, count);
    return out;
}

void Job::recievedData(std::string data) {
    std::stringstream ss = std::stringstream(data);
    ss >> name >> res >> max >> pos >> count;
    stop = false;
}
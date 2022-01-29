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
    ss >> name >> res >> max >> pos;
    srand(time(0));
    stop = false;
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
    res = (float)count / (float)max;
    return res;
}

std::string Job::toString() {
    char out[2048];
    sprintf(out, "%s %f %d %d %d %d", name, res, max, pos, count);
    return out;
}
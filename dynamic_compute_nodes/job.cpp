#include "job.hpp"
#include <sstream>
#include <string>
#include <fstream>
#include <cstdio>

Job::Job(std::string name)
{
    std::string args;
    std::string name;
    system(name.c_str());
}

Job::~Job()
{
}

Job::Job(char* prevJob) {
    std::stringstream ss(prevJob);
    std::string buffer;
    std::string args;
    std::string name;
    getline(ss, name);
    getline(ss, args);
    std::ofstream outputFile;
    outputFile.open(name);
    while (ss >> buffer)
    {
        
    }
    
}


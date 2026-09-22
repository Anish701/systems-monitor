#ifndef MONITOR_PROCESS
#define MONITOR_PROCESS

#include <string>
#include <vector>

struct Process {
    int pid;
    std::string name;
    float cpu_usage;
    float memory_usage;
};

std::vector<Process> get_processes();

#endif
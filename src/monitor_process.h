#ifndef MONITOR_PROCESS
#define MONITOR_PROCESS

#include <string>
#include <vector>

struct Process {
    int pid;
    int parent_pid;
    std::string name;
    float cpu_usage;
    float memory_usage;
};

bool process_info(pid_t pid, std::string& name, int& parent_pid);

float process_cpu_usage(pid_t pid);

float process_memory_usage(pid_t pid);

std::vector<Process> get_processes();

std::vector<Process> group_processes(const std::vector<Process>& processes);

#endif
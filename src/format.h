#ifndef FORMAT_H
#define FORMAT_H

#include <string>

#include "monitor_usage.h"
#include "monitor_process.h"

std::string percentage(float fraction);

void print_usage(UsageData cpu_usage_data, UsageData memory_usage_data);

void print_processes(std::vector<Process> processes);

#endif
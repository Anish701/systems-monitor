#include <chrono>
#include <thread>
#include <iostream>
#include <vector>

#include "monitor_usage.h"
#include "format.h"
#include "monitor_process.h"

using namespace std;

int main() {
    cpu_usage();
    get_processes();
    this_thread::sleep_for(chrono::milliseconds(500));

    UsageData cpu_usage_data = cpu_usage();
    UsageData memory_usage_data = memory_usage();

    print_usage(cpu_usage_data, memory_usage_data);

    vector<Process> res = get_processes();
    for (const auto& element : res) {
        std::cout << element.pid << " " << element.name << " " << element.cpu_usage << " " << element.memory_usage << endl;
    }
    
    return 0;
}
#include <chrono>
#include <thread>
#include <vector>

#include "monitor_usage.h"
#include "format.h"
#include "monitor_process.h"

using namespace std;

int main() {
    cpu_usage();
    get_processes();
    this_thread::sleep_for(chrono::seconds(2));

    UsageData cpu_usage_data = cpu_usage();
    UsageData memory_usage_data = memory_usage();

    print_usage(cpu_usage_data, memory_usage_data);

    vector<Process> processes = get_processes();
    
    print_processes(processes);
    
    return 0;
}
#include <chrono>
#include <thread>

#include "monitor_usage.h"
#include "format.h"

using namespace std;

int main() {
    cpu_usage();
    this_thread::sleep_for(chrono::milliseconds(500));

    UsageData cpu_usage_data = cpu_usage();
    UsageData memory_usage_data = memory_usage();

    print_usage(cpu_usage_data, memory_usage_data);
    
    return 0;
}
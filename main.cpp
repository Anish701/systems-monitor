#include <iostream>
#include <chrono>
#include <thread>

#include "monitor_usage.h"
#include "format.h"

using namespace std;

int main() {

    float memory_usage_ratio = memory_usage();

    cpu_usage();

    this_thread::sleep_for(chrono::milliseconds(500));

    float cpu_usage_ratio = cpu_usage();

    cout << "Memory Usage: " << percentage(memory_usage_ratio) << endl;
    cout << "CPU Usage: " << percentage(cpu_usage_ratio) << endl;

    return 0;
}
#include <format>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "format.h"

using namespace std;

string percentage(float fraction) {
    return format("{:.2f}%", fraction * 100);
}

string memory_size(float gb) {
    if(gb >= 1.0) {
        return format("{:.2f} GB", gb);
    }

    return format("{:.0f} MB", gb * 1024);
}

void print_usage(UsageData cpu_usage_data, UsageData memory_usage_data) {
    cout << "TOTAL MACHINE STATS" << endl;
    cout << "RAM: " << format("{:.0f}", memory_usage_data.used) << " GB / " 
                    << format("{:.0f}", memory_usage_data.total) << " GB" << endl;
    cout << "CPU: " << percentage(cpu_usage_data.usage_ratio) << endl;
    cout << endl;
}

void print_processes(vector<Process> processes) {
    sort(processes.begin(), processes.end(),
        [](const Process& a, const Process& b) {
            return a.cpu_usage > b.cpu_usage;
        }
    );

    cout << left
         << setw(10) << "PID"
         << setw(40) << "NAME"
         << setw(15) << "CPU (%core)"
         << setw(15) << "MEMORY"
         << endl;

    cout << string(74, '-') << endl;

    int count = min(10, (int)processes.size());

    for (int i = 0; i < count; i++) {
        const auto& process = processes[i];

        cout << left
            << setw(10) << process.pid
            << setw(40) << process.name
            << setw(15) << percentage(process.cpu_usage)
            << setw(15) << memory_size(process.memory_usage)
            << endl;
    }
}
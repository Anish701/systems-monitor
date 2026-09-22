#include <format>
#include <iostream>

#include "format.h"

using namespace std;

string percentage(float fraction) {
    return format("{:.2f}%", fraction * 100);
}

void print_usage(UsageData cpu_usage_data, UsageData memory_usage_data) {
    cout << "RAM: " << percentage(memory_usage_data.usage_ratio) << " - " 
         << format("{:.0f}", memory_usage_data.used_gb) << " GB / " << format("{:.0f}", memory_usage_data.total_gb) << " GB" << endl;
    cout << "CPU: " << percentage(cpu_usage_data.usage_ratio) << " - " 
         << cpu_usage_data.used_gb << " / " << cpu_usage_data.total_gb << endl;
}
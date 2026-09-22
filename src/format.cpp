#include <format>
#include <iostream>

#include "format.h"

using namespace std;

string percentage(float fraction) {
    return format("{:.2f}%", fraction * 100);
}

void print_usage(UsageData cpu_usage_data, UsageData memory_usage_data) {
    cout << "RAM: " << percentage(memory_usage_data.usage_ratio) << endl;
    cout << "CPU: " << percentage(cpu_usage_data.usage_ratio) << endl;
}
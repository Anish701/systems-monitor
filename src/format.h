#ifndef FORMAT_H
#define FORMAT_H

#include <string>

#include "monitor_usage.h"

std::string percentage(float fraction);

void print_usage(UsageData cpu_usage_data, UsageData memory_usage_data);

#endif
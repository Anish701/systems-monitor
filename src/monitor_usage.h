#ifndef MONITOR_USAGE_H
#define MONITOR_USAGE_H

struct UsageData {
    float total;
    float used;
    float usage_ratio;
};

UsageData memory_usage();

UsageData cpu_usage();

#endif
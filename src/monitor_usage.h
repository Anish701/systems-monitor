#ifndef MONITOR_USAGE_H
#define MONITOR_USAGE_H

struct UsageData {
    float total_gb;
    float used_gb;
    float usage_ratio;
};

UsageData memory_usage();

UsageData cpu_usage();

#endif
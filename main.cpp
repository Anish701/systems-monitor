#include <iostream>

#include "memory_usage.h"

int main() {

    float memory_usage_percentage = memory_usage();

    std::cout << "Memory Usage: " << memory_usage_percentage << std::endl;

    return 0;
}
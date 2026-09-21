#include <iostream>

#include "memory_usage.h"
#include "format.h"

using namespace std;

int main() {

    float memory_usage_ratio = memory_usage();
    string memory_usage_percentage = percentage(memory_usage_ratio);

    cout << "Memory Usage: " << memory_usage_percentage << endl;

    return 0;
}
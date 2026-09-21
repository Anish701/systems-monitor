#include "format.h"
#include <format>

using namespace std;

string percentage(float fraction) {
    return format("{:.2f}%", fraction * 100);
}
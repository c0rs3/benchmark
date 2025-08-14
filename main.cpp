#include <timer.h>
#include <iostream>

int main() {
    // std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    // std::chrono::steady_clock::time_point later = std::chrono::steady_clock::now();
    // std::cout << std::chrono::duration_cast<std::chrono::seconds>(later - now).count();
    benchtools::Timer a{benchtools::timeunits::Default};
    return 0;
}
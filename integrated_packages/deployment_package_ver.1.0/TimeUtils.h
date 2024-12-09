#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <chrono>
#include <string>

std::chrono::time_point<std::chrono::system_clock> timePointFromStr(
    std::string
);

std::string timePointToStr(
    std::chrono::time_point<std::chrono::system_clock>
);

#endif // TIME_UTILS_H


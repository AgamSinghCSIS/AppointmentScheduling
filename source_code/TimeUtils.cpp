#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "TimeUtils.h"

std::chrono::time_point<std::chrono::system_clock> timePointFromStr(
    std::string dateStr = "2024-04-21 12:30:00"
) {
    std::tm timeStruct = {};
    std::stringstream ss(dateStr);

    ss >> std::get_time(&timeStruct, "%Y-%m-%d %H:%M:%S");

    auto out_time_t = std::mktime(&timeStruct);
    return std::chrono::system_clock::from_time_t(out_time_t);
}
std::string timePointToStr(
    std::chrono::time_point<std::chrono::system_clock> t
) {
    auto in_time_t = std::chrono::system_clock::to_time_t(t);

    std::tm *timeStruct = std::localtime(&in_time_t);

    if (timeStruct->tm_isdst) {
        timeStruct->tm_hour -= 1;
    }

    std::stringstream ss;
    ss << std::put_time(timeStruct, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}


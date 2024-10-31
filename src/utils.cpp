#include "utils.h"
#include <iomanip>
#include <sstream>

// Convert time from ms to mm:ss:ms 
std::string formatTimeFromMilliseconds_32(uint32_t totalMilliseconds) {

    int minutes = totalMilliseconds / 60000; // 60,000 ms in a minute
    int seconds = (totalMilliseconds % 60000) / 1000; // 1,000 ms in a second
    int milliseconds = totalMilliseconds % 1000; // Remaining milliseconds

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << minutes << ":"
        << std::setw(2) << std::setfill('0') << seconds << "."
        << std::setw(3) << std::setfill('0') << milliseconds;

    return oss.str();
}

// Convert time from ms to mm:ss:ms but ms is in uint16_t
std::string formatTimeFromMilliseconds_16(uint16_t totalMilliseconds) {

    return formatTimeFromMilliseconds_32(static_cast<uint32_t>(totalMilliseconds));
}

// Convert time from seconds to mm:ss:ms

std::string formatTimeFromSeconds(double totalTime) {

    int minutes = static_cast<int>(totalTime / 60); 
    int seconds = static_cast<int>(totalTime) % 60;
    int milliseconds = static_cast<int>((totalTime - static_cast<int>(totalTime)) * 1000);

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << minutes << ":"
        << std::setw(2) << std::setfill('0') << seconds << "."
        << std::setw(3) << std::setfill('0') << milliseconds;
    return oss.str();
}

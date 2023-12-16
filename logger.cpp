#include "logger.h"

#include <map>
#include <string>
#include <utility>

namespace casino {
namespace logger {

    LEVEL GLOBAL_DEFAULT_LOG_LEVEL = WARNING;

    const std::map<COLOR, std::pair<int, int>> Logger::COLOR_VALUE_MAP = {
        { GREEN, { 0, 32 } },
        { WHITE, { 0, 30 } },
        { YELLOW, { 0, 33 } },
        { RED, { 0, 31 } },
        { BOLD_RED, { 1, 41 } }
    };

    const std::map<LEVEL, COLOR> Logger::LEVEL_COLOR_MAP = {
        { DEBUG, GREEN },
        { INFO, WHITE },
        { WARNING, YELLOW },
        { ERROR, RED },
        { SEVERE, BOLD_RED }
    };

    const std::map<LEVEL, std::string> Logger::LEVEL_NAME_MAP = {
        { DEBUG, "DEBUG" },
        { INFO, "INFO" },
        { WARNING, "WARN" },
        { ERROR, "ERROR" },
        { SEVERE, "SEVERE" }
    };

    std::string Logger::getName() const
    {
        return myName;
    }

    void Logger::resetLogLevel()
    {
        myUseOverriddenLogLevel = false;
    }

    void Logger::setLogLevel(LEVEL pLogLevel)
    {
        myUseOverriddenLogLevel = true;
        myOverriddenLogLevel = pLogLevel;
    }

    std::string Logger::_timestamp_string() const
    {
        time_t timeNow = time(NULL);
        char timeStr[100];
        strftime(timeStr, sizeof(timeStr), "%Y%m%d:%T:%Z", localtime(&timeNow));
        return std::string(timeStr);
    }

    std::string Logger::_colored_string(COLOR color, std::string pStr) const
    {
        std::pair<int, int> colorPair;
        try {
            colorPair = COLOR_VALUE_MAP.at(color);
        } catch (std::string err) {
            return pStr;
        }
        return "\033["
            + std::to_string(colorPair.first)
            + ";"
            + std::to_string(colorPair.second)
            + "m"
            + pStr
            + "\033[0m";
    }

    const Logger logger { "global-logger" };

}
}

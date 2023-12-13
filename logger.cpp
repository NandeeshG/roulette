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

}
}

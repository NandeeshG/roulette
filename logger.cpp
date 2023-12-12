#include "logger.h"

#include <map>
#include <string>
#include <utility>

namespace casino {
namespace logger {

    const std::map<COLOR, std::pair<int, int>> Logger::COLOR_VALUE_MAP = {
        { BOLD_RED, { 1, 41 } },
        { RED, { 0, 31 } },
        { YELLOW, { 0, 33 } },
        { WHITE, { 0, 30 } },
        { GREEN, { 0, 32 } }
    };

    const std::map<LEVEL, COLOR> Logger::LEVEL_COLOR_MAP = {
        { SEVERE, BOLD_RED },
        { ERROR, RED },
        { WARNING, YELLOW },
        { INFO, WHITE },
        { DEBUG, GREEN }
    };

    const std::map<LEVEL, std::string> Logger::LEVEL_NAME_MAP = {
        { SEVERE, "SEVERE" },
        { ERROR, "ERROR" },
        { WARNING, "WARN" },
        { INFO, "INFO" },
        { DEBUG, "DEBUG" }
    };

}
}

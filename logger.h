#ifndef CASINO_LOGGER_H
#define CASINO_LOGGER_H

#include <iostream>
#include <map>
#include <string>
#include <utility>

namespace casino {
namespace logger {

    enum COLOR {
        BOLD_RED,
        RED,
        YELLOW,
        GREEN,
        WHITE
    };

    enum LEVEL {
        SEVERE,
        ERROR,
        WARNING,
        INFO,
        DEBUG,
    };

    class Logger {
    private:
        static const std::map<LEVEL, std::string> LEVEL_NAME_MAP;
        static const std::map<LEVEL, COLOR> LEVEL_COLOR_MAP;
        static const std::map<COLOR, std::pair<int, int>> COLOR_VALUE_MAP;

        std::string myName;
        LEVEL myLogLevel;

        std::string _timestamp_string()
        {
            time_t timeNow = time(NULL);
            char timeStr[100];
            strftime(timeStr, sizeof(timeStr), "%Y%m%d:%T:%Z", localtime(&timeNow));
            return std::string(timeStr);
        }

        std::string _colored_string(COLOR color, std::string pStr)
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

        template <typename... T>
        void _f_print(std::string pLevel, T... pStrs)
        {
            std::cout << '[' << _timestamp_string() << ']'
                      << '[' << myName << ']'
                      << '[' << pLevel << ']'
                      << ':';
            (void)(int[]) { (std::cout << pStrs << ' ', 0)... };
            std::cout << '\n';
        }

    public:
        Logger()
        {
            myName = "unknown";
            myLogLevel = WARNING;
        }

        Logger(std::string pName)
            : myName(pName)
        {
            myLogLevel = WARNING;
        }

        Logger(std::string pName, LEVEL pLogLevel)
            : myName(pName)
            , myLogLevel(pLogLevel)
        {
        }

        std::string getName()
        {
            return myName;
        }

        void setLogLevel(LEVEL pLogLevel)
        {
            myLogLevel = pLogLevel;
        }

        template <typename... Args>
        void log(LEVEL logLevel, Args... pStrs)
        {
            if (myLogLevel <= logLevel) {
                _f_print(_colored_string(
                             LEVEL_COLOR_MAP.at(logLevel),
                             LEVEL_NAME_MAP.at(logLevel)),
                    pStrs...);
            }
        }

        template <typename... Args>
        void debug(Args... pStrs)
        {
            log(DEBUG, pStrs...);
        }

        template <typename... Args>
        void info(Args... pStrs)
        {
            log(INFO, pStrs...);
        }

        template <typename... Args>
        void warning(Args... pStrs)
        {
            log(WARNING, pStrs...);
        }

        template <typename... Args>
        void error(Args... pStrs)
        {
            log(ERROR, pStrs...);
        }

        template <typename... Args>
        void severe(Args... pStrs)
        {
            log(SEVERE, pStrs...);
        }
    };

} // namespace logger
} // namespace casino

#endif // CASINO_LOGGER_H

#ifndef CASINO_LOGGER_H
#define CASINO_LOGGER_H

#include <iostream>
#include <map>
#include <string>
#include <utility>

namespace casino {
namespace logger {

    enum COLOR {
        GREEN,
        WHITE,
        YELLOW,
        RED,
        BOLD_RED
    };

    enum LEVEL {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        SEVERE
    };

    extern LEVEL GLOBAL_DEFAULT_LOG_LEVEL;

    class Logger {
    private:
        static const std::map<LEVEL, std::string> LEVEL_NAME_MAP;
        static const std::map<LEVEL, COLOR> LEVEL_COLOR_MAP;
        static const std::map<COLOR, std::pair<int, int>> COLOR_VALUE_MAP;

        std::string myName;
        bool myUseOverriddenLogLevel;
        LEVEL myOverriddenLogLevel;

        std::string _timestamp_string() const;

        std::string _colored_string(COLOR color, std::string pStr) const;

        template <typename... T>
        void _f_print(std::string pLevel, T... pStrs) const
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
            : myName("unknown")
            , myUseOverriddenLogLevel(false)
        {
        }

        Logger(std::string pName)
            : myName(pName)
            , myUseOverriddenLogLevel(false)
        {
        }

        Logger(std::string pName, LEVEL pLogLevel)
            : myName(pName)
            , myUseOverriddenLogLevel(true)
            , myOverriddenLogLevel(pLogLevel)
        {
        }

        std::string getName() const;

        void setLogLevel(LEVEL pLogLevel);

        void resetLogLevel();

        template <typename... Args>
        void log(LEVEL logLevel, Args... pStrs) const
        {
            LEVEL lvlToUse;
            if (myUseOverriddenLogLevel)
                lvlToUse = myOverriddenLogLevel;
            else
                lvlToUse = GLOBAL_DEFAULT_LOG_LEVEL;

            if (lvlToUse <= logLevel) {
                _f_print(_colored_string(
                             LEVEL_COLOR_MAP.at(logLevel),
                             LEVEL_NAME_MAP.at(logLevel)),
                    pStrs...);
            }
        }

        template <typename... Args>
        void debug(Args... pStrs) const
        {
            log(DEBUG, pStrs...);
        }

        template <typename... Args>
        void info(Args... pStrs) const
        {
            log(INFO, pStrs...);
        }

        template <typename... Args>
        void warning(Args... pStrs) const
        {
            log(WARNING, pStrs...);
        }

        template <typename... Args>
        void error(Args... pStrs) const
        {
            log(ERROR, pStrs...);
        }

        template <typename... Args>
        void severe(Args... pStrs) const
        {
            log(SEVERE, pStrs...);
            exit(100 + SEVERE);
        }

        template <typename... Args>
        void severe(int errorCode, Args... pStrs) const
        {
            log(SEVERE, pStrs...);
            exit(errorCode);
        }
    };

    extern const Logger logger;

} // namespace logger
} // namespace casino

#endif // CASINO_LOGGER_H

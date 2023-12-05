#ifndef CASINO_LOGGER_H
#define CASINO_LOGGER_H

#include <iostream>
#include <string>

namespace casino {
namespace logger {

    enum Level {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        SEVERE
    };

    class Logger {
    private:
        std::string mName;
        Level mLogLevel;

        std::string get_timestamp();

        std::string bold_red(std::string);
        std::string red(std::string);
        std::string yellow(std::string);
        std::string green(std::string);

        template <typename... Args>
        void m_print(std::string pLevel, Args... pStrs);

    public:
        Logger();
        Logger(std::string);
        Logger(std::string, Level);

        std::string getName();
        void setLogLevel(Level);

        template <typename... Args>
        void debug(Args...);

        template <typename... Args>
        void info(Args...);

        template <typename... Args>
        void warning(Args...);

        template <typename... Args>
        void error(Args...);

        template <typename... Args>
        void severe(Args...);
    };

    std::string Logger::get_timestamp()
    {
        time_t time_now = time(NULL);
        char time_str[100];
        strftime(time_str, sizeof(time_str), "%Y%m%d:%T:%Z", localtime(&time_now));
        return std::string(time_str);
    }

    std::string Logger::bold_red(std::string pStr)
    {
        return "\033[1;41m" + pStr + "\033[0m";
    }

    std::string Logger::red(std::string pStr)
    {
        return "\033[0;31m" + pStr + "\033[0m";
    }

    std::string Logger::yellow(std::string pStr)
    {
        return "\033[0;33m" + pStr + "\033[0m";
    }

    std::string Logger::green(std::string pStr)
    {
        return "\033[0;32m" + pStr + "\033[0m";
    }

    template <typename... T>
    void Logger::m_print(std::string pLevel, T... pStrs)
    {
        std::cout << '[' << get_timestamp() << ']'
                  << '[' << mName << ']'
                  << '[' << pLevel << ']'
                  << ':';
        (void)(int[]) { (std::cout << pStrs << ' ', 0)... };
        std::cout << '\n';
    }

    Logger::Logger()
    {
        mName = "unknown";
        mLogLevel = WARNING;
    }

    Logger::Logger(std::string pName)
        : mName(pName)
    {
        mLogLevel = WARNING;
    }

    Logger::Logger(std::string pName, Level pLogLevel)
        : mName(pName)
        , mLogLevel(pLogLevel)
    {
    }

    std::string Logger::getName()
    {
        return mName;
    }

    void Logger::setLogLevel(Level pLogLevel)
    {
        mLogLevel = pLogLevel;
    }

    template <typename... Args>
    void Logger::debug(Args... pStrs)
    {
        if (mLogLevel <= DEBUG)
            m_print(green("DEBUG"), pStrs...);
    }

    template <typename... Args>
    void Logger::info(Args... pStrs)
    {
        if (mLogLevel <= INFO)
            m_print("INFO", pStrs...);
    }

    template <typename... Args>
    void Logger::warning(Args... pStrs)
    {
        if (mLogLevel <= WARNING)
            m_print(yellow("WARNING"), pStrs...);
    }

    template <typename... Args>
    void Logger::error(Args... pStrs)
    {
        if (mLogLevel <= ERROR)
            m_print(red("ERROR"), pStrs...);
    }

    template <typename... Args>
    void Logger::severe(Args... pStrs)
    {
        if (mLogLevel <= SEVERE)
            m_print(bold_red("SEVERE"), pStrs...);
    }

} // namespace logger
} // namespace casino

#endif // CASINO_LOGGER_H
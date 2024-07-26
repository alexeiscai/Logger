/**
 * @file Logger.h
 *
 * @brief This file defines the Logger class, 
 * which provides a singleton-based logging utility with different log levels.
 *
 * @author A Scai
 *
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <format> 
#include <filesystem>
#include <fstream>
#include <cstring>

/* Define a macro to conditionally change access level */
#ifdef TEST_MODE
#define PROTECTED public
#else
#define PROTECTED protected
#endif

enum class LogLevel
{
    DEBUG = 0,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};


class Logger
{
public:

    Logger(Logger &other) = delete;

    Logger& operator=(const Logger&) = delete;

    ~Logger();

    static Logger& getInstance();

    void setLogLevel(LogLevel logPriority);

    LogLevel getLogLevel() const;

    /**
     * moving variadic template function to .cpp file
     * requires explicit instantiation declaration of 
     * any possible args type and method signature.
     */
    template<typename... Args>
    void log(const std::string &fileName, const int &codeLine, LogLevel logLevelArg, const std::string& msg, Args&&... args)
    {
        std::string msgStr = "[" + fileName + ":" + std::to_string(codeLine) + "] " + msg;
        std::string formattedMsg   = std::vformat(msgStr, std::make_format_args(std::forward<Args>(args)...));
        addLog(logLevelArg, formattedMsg);
    }

private:

    static std::unique_ptr<Logger> s_loggerInstance;

    static std::mutex mutex_;

    static LogLevel s_logLevel;

    std::ofstream logFile_;

    std::filesystem::path fullPath_;


PROTECTED:

    std::string getCurrentTime();

    void initLog();

    void addLog(LogLevel logLevelArg, const std::string& msg);

    Logger();
};

#define __FILENAME__  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOG_DEBUG(message, ...) (Logger::getInstance().log(__FILENAME__, __LINE__, LogLevel::DEBUG, message, ##__VA_ARGS__))
#define LOG_INFO(message, ...) (Logger::getInstance().log(__FILENAME__, __LINE__, LogLevel::INFO, message, ##__VA_ARGS__))
#define LOG_WARNING(message, ...) (Logger::getInstance().log(__FILENAME__, __LINE__, LogLevel::WARNING, message, ##__VA_ARGS__))
#define LOG_ERROR(message, ...) (Logger::getInstance().log(__FILENAME__, __LINE__, LogLevel::ERROR, message, ##__VA_ARGS__))
#define LOG_CRITICAL(message, ...) (Logger::getInstance().log(__FILENAME__, __LINE__, LogLevel::CRITICAL, message, ##__VA_ARGS__))

#endif
/**
 * @file Logger.cpp
 *
 * @brief This file contains the implementation of the Logger class methods.
 * 
 * The Logger class is a singleton that manages logging functionality with various
 * log levels (DEBUG, INFO, WARNING, ERROR, CRITICAL). It ensures thread safety
 * using a mutex and supports logging messages to a file with timestamps and
 * formatted output.
 *
 * @author A Scai
 *
 */

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <mutex>

#include "Logger.h"

LogLevel Logger::s_logLevel = LogLevel::DEBUG;
std::unique_ptr<Logger> Logger::s_loggerInstance = nullptr;
std::mutex Logger::mutex_;

Logger::Logger(){}

Logger::~Logger()
{
    std::lock_guard<std::mutex> lock(mutex_);

    if (logFile_.is_open())
    {
        logFile_.close();
    }
}

Logger& Logger::getInstance()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (s_loggerInstance == nullptr)
    {
        s_loggerInstance.reset(new Logger());

        s_loggerInstance->initLog();
    }

    return *s_loggerInstance;
}

void Logger::initLog()
{
    std::filesystem::path path = std::filesystem::path("./");
    std::filesystem::path logFileName = "log_messages.log";

    fullPath_ = path / logFileName;

    logFile_.open(fullPath_, std::ios::out | std::ios::app);
    if (!logFile_.is_open())
    {
        throw std::runtime_error("Unable to open log file: " + fullPath_.string());
    }
}

void Logger::setLogLevel(LogLevel logLevel)
{
	s_logLevel = logLevel;
}

LogLevel Logger::getLogLevel() const 
{
    return s_logLevel;
}

std::string Logger::getCurrentTime()
{
	std::tm now_tm;
	std::stringstream ssTime;
    const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    const std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

    if (localtime_r(&now_time_t, &now_tm) == nullptr)
	{
        return std::string("Error: Unable to get current time.");
    }

    ssTime << std::put_time(&now_tm, "[%Y-%m-%d %H:%M:%S]");

    return ssTime.str();
}

void Logger::addLog(LogLevel logLevelArg, const std::string& msg)
{
    std::lock_guard<std::mutex> lock(mutex_);

    std::string logLevelStr = "";

    if (!logFile_.is_open())
    {
        logFile_.open(fullPath_, std::ios::out | std::ios::app);
    }

    switch (logLevelArg)
    {
        case LogLevel::DEBUG:
        {
            logLevelStr = "[DEBUG]";
            break;
        }
        case LogLevel::INFO:
        {
            logLevelStr = "[INFO]";
            break;
        }
        case LogLevel::WARNING:
        {
            logLevelStr = "[WARNING]";
            break;
        }
        case LogLevel::ERROR:
        {
            logLevelStr = "[ERROR]";
            break;
        }
        case LogLevel::CRITICAL:
        {
            logLevelStr = "[CRITICAL]";
            break;
        }
        default:
        {
            logFile_ << "[LogLevel:: UNKNOWN] " << msg << std::endl;
            std::cerr << "Unhandled LogLevel!" << std::endl;
            break;
        }
    }

    std::string time = getCurrentTime();
    logFile_ << time << logLevelStr << msg << std::endl;
    std::cout << time << logLevelStr << msg << std::endl;
}


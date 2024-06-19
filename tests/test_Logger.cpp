#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include <regex>

#define TEST_MODE

#include "Logger.h"

class LoggerTestInst : public ::testing::Test {};


class LoggerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        std::filesystem::remove(logFilePath);
    }

    void TearDown() override
    {
        std::filesystem::remove(logFilePath);
    }

    std::string readLogFile()
    {
        std::string ret;

        if(std::filesystem::exists(logFilePath))
        {
            std::ifstream logFile(logFilePath);
            std::stringstream buffer;
            buffer << logFile.rdbuf();
            ret = buffer.str();
        }

        return ret;
    }

    std::vector<std::string> splitStreamData(const std::string& logContent)
    {
        std::string line;
        std::vector<std::string> logEntries;
        std::istringstream stream(logContent);
        while (std::getline(stream, line))
        {
            logEntries.push_back(line);
        }
        return logEntries;
    }

    std::filesystem::path logFilePath = std::filesystem::current_path() / "log_messages.log";
};

TEST_F(LoggerTest, logFilePath_TC1)
{
    LOG_INFO("Formatted info message with int: {} and string: {}", 42, "test");
    LOG_DEBUG("Formatted debug message with float: {:.2f} and char: {}", 3.14, 'A');

    std::string logContent = readLogFile();
    auto logEntries = splitStreamData(logContent);

    ASSERT_EQ(logEntries.size(), 2);
    EXPECT_NE(logEntries[0].find("Formatted info message with int: 42 and string: test"), std::string::npos);
    EXPECT_NE(logEntries[1].find("Formatted debug message with float: 3.14 and char: A"), std::string::npos);
}

TEST_F(LoggerTestInst, getnstance_TC2)
{
    Logger* instance1 = &Logger::getInstance();
    Logger* instance2 = &Logger::getInstance();
    ASSERT_EQ(instance1, instance2);
}


TEST_F(LoggerTestInst, timestampFormat_TC3)
{
    std::string timestamp = Logger::getInstance().getCurrentTime();
    std::regex timestampRegex(R"(\[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\])");
    EXPECT_TRUE(std::regex_match(timestamp, timestampRegex));
}

TEST_F(LoggerTestInst, getCurrentTime_TC4)
{
    std::string currentTime = Logger::getInstance().getCurrentTime();
    EXPECT_NE(currentTime.find("["), std::string::npos);
    EXPECT_NE(currentTime.find("]"), std::string::npos);
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
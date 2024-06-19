#include <mutex>
#include <thread>
#include <chrono>

#include "Logger.h"

void threadFoo()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    int errorCode = 404;
    std::string errorMessage = "Not Found";

    LOG_ERROR("Error code: {}, message: {}", errorCode, errorMessage);
    LOG_DEBUG("Log meessage and optional param {}", 23);
}

int main()
{
    std::thread t1(threadFoo);
    t1.join();

    return 0;
}
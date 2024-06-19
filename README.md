# Simple thread-safe logger.

# Usage example:

    int errorCode = 404;
    std::string errorMessage = "Not Found";

    LOG_ERROR("Error code: {}, message: {}", errorCode, errorMessage);
    LOG_DEBUG("Log meessage and optional param {}", 23);

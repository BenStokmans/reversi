#ifndef REVERSI_LOGGER_H
#define REVERSI_LOGGER_H

#include <string>
#include <filesystem>
#include <vector>
#include <fstream>
#include <sstream>
#include <cassert>
#include <iostream>
#include <chrono>

class Logger {
public:
    void create(const std::string& verbosity);
    void write(std::string_view verbosity, std::string_view source, std::string_view message);

    void trace(std::string_view source, std::string_view message);
    void debug(std::string_view source, std::string_view message);
    void error(std::string_view source, std::string_view message);
    void info(std::string_view source, std::string_view message);
    void fatal(std::string_view source, std::string_view message);
    void trace(std::string_view message);
    void debug(std::string_view message);
    void error(std::string_view message);
    void info(std::string_view message);
    void fatal(std::string_view message);
private:
    std::vector<std::string> levels;

    std::filesystem::path filePath;
    int currentVerbosityIndex;
};
#define log_source (std::string() + __func__)

#define LOG_TRACE(x) logger.trace(log_source, x)
#define LOG_INFO(x) logger.info(log_source, x)
#define LOG_DEBUG(x) logger.debug(log_source, x)
#define LOG_ERROR(x) logger.error(log_source, x)
#define LOG_FATAL(x) \
    logger.fatal(log_source, x);  \
    exit(1);

#define STREAM_TRACE(x, y) \
	do { \
		std::ostringstream ss; \
		ss << y; \
		logger.trace(x, ss.str()); \
	} while (0)

#define STREAM_DEBUG(x,y) \
	do { \
		std::ostringstream ss; \
		ss << y; \
		logger.debug(x, ss.str()); \
	} while (0)

#define STREAM_ERROR(x,y) \
	do { \
		std::ostringstream ss; \
		ss << y; \
		logger.debug(x, ss.str()); \
	} while (0)

#define STREAM_FATAL(x, y) \
	do { \
		std::ostringstream ss; \
		ss << y; \
		logger.fatal(x, ss.str()); \
	} while (0)

#define LOG_ASSERT(EXPR) \
	if (!(EXPR)) { \
        STREAM_FATAL(log_source, "(" << #EXPR << ") assert failed"); \
        assert(EXPR); \
        exit(1); \
    }

extern Logger logger;


#endif //REVERSI_LOGGER_H
